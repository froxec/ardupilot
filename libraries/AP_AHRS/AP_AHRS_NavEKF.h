#ifndef __AP_AHRS_NAVEKF_H__
#define __AP_AHRS_NAVEKF_H__
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *  NavEKF based AHRS (Attitude Heading Reference System) interface for
 *  ArduPilot
 *
 */

#include <AP_AHRS.h>

#if HAL_CPU_CLASS >= HAL_CPU_CLASS_150
#include <AP_NavEKF.h>

#define AP_AHRS_NAVEKF_AVAILABLE 1

class AP_AHRS_NavEKF : public AP_AHRS_DCM
{
public:
    // Constructor
    AP_AHRS_NavEKF(AP_InertialSensor &ins, AP_Baro &baro, GPS *&gps) :
        AP_AHRS_DCM(ins, baro, gps),
        EKF(this, baro),
        _baro(baro),
        ekf_started(false),
        startup_delay_ms(5000)
        {
        }

    // return the smoothed gyro vector corrected for drift
    const Vector3f get_gyro(void) const;
    const Matrix3f &get_dcm_matrix(void) const;

    // return the current drift correction integrator value
    const Vector3f &get_gyro_drift(void) const;

    void            update(void);
    void            reset(bool recover_eulers = false);

    void            set_correct_centrifugal(bool setting);

    // reset the current attitude, used on new IMU calibration
    void reset_attitude(const float &roll, const float &pitch, const float &yaw);

    // dead-reckoning support
    bool get_position(struct Location &loc);

    // status reporting of estimated error
    float           get_error_rp(void);
    float           get_error_yaw(void);

    // return a wind estimation vector, in m/s
    Vector3f wind_estimate(void);

    // return an airspeed estimate if available. return true
    // if we have an estimate
    bool airspeed_estimate(float *airspeed_ret);

    // true if compass is being used
    bool use_compass(void);

    NavEKF &get_NavEKF(void) { return EKF; }

    // return secondary attitude solution if available, as eulers in radians
    bool get_secondary_attitude(Vector3f &eulers);

    // return secondary position solution if available
    bool get_secondary_position(struct Location &loc);

    // EKF has a better ground speed vector estimate
    Vector2f groundspeed_vector(void);

    // set home location
    void set_home(int32_t lat, int32_t lng, int32_t alt_cm);

    bool have_inertial_nav(void) const;

    bool get_velocity_NED(Vector3f &vec) const;
    bool get_relative_position_NED(Vector3f &vec) const;

    void set_ekf_use(bool setting) { _ekf_use.set(setting); }

private:
    bool using_EKF(void) const;

    NavEKF EKF;
    AP_Baro &_baro;
    bool ekf_started;
    Matrix3f _dcm_matrix;
    Vector3f _dcm_attitude;
    const uint16_t startup_delay_ms;
    uint32_t start_time_ms;
};
#endif

#endif // __AP_AHRS_NAVEKF_H__
