#ifndef __NMEA__AAM__HPP__
#define __NMEA__AAM__HPP__

#include <marnav/nmea/sentence.hpp>
#include <marnav/nmea/waypoint.hpp>
#include <marnav/utils/optional.hpp>

namespace marnav
{
namespace nmea
{
MARNAV_NMEA_DECLARE_SENTENCE_PARSE_FUNC(aam)

/// @brief AAM - Waypoint Arrival Alarm
///
/// This sentence is generated by some units to indicate the status of arrival (entering
/// the arrival circle, or passing the perpendicular of the course line) at the destination
/// waypoint.
///
/// @code
///        1 2 3   4 5
///        | | |   | |
/// $--AAM,A,A,x.x,N,c--c*hh<CR><LF>
/// @endcode
///
/// Field Number:
/// 1. Status
///    - A = arrival circle entered
///    - V = arrival circle not entered
/// 2. Status
///    - A = perpendicular passed at waypoint
///    - V = perpendicular not passed
/// 3. Arrival circle radius
/// 4. Units of radius
///    - N = nautical miles
/// 5. Waypoint ID
///
/// Example: <tt>$GPAAM,A,A,0.10,N,WPTNME*43</tt>
///
/// `WPTNME` is the waypoint name.
///
class aam : public sentence
{
	MARNAV_NMEA_SENTENCE_FRIENDS(aam)

public:
	constexpr static const sentence_id ID = sentence_id::AAM;
	constexpr static const char * TAG = "AAM";

	aam();
	aam(const aam &) = default;
	aam & operator=(const aam &) = default;
	aam(aam &&) = default;
	aam & operator=(aam &&) = default;

protected:
	aam(const std::string & talker, fields::const_iterator first, fields::const_iterator last);
	virtual std::vector<std::string> get_data() const override;

private:
	utils::optional<status> arrival_circle_entered;
	utils::optional<status> perpendicualar_passed;
	utils::optional<double> arrival_circle_radius;
	utils::optional<unit::distance> arrival_circle_radius_unit;
	utils::optional<waypoint> waypoint_id;

	void check() const;

public:
	decltype(arrival_circle_entered) get_arrival_circle_entered() const
	{
		return arrival_circle_entered;
	}
	decltype(perpendicualar_passed) get_perpendicualar_passed() const
	{
		return perpendicualar_passed;
	}
	decltype(arrival_circle_radius) get_arrival_circle_radius() const
	{
		return arrival_circle_radius;
	}
	decltype(arrival_circle_radius_unit) get_arrival_circle_radius_unit() const
	{
		return arrival_circle_radius_unit;
	}
	decltype(waypoint_id) get_waypoint_id() const { return waypoint_id; }

	void set_arrival_circle_entered(status s);
	void set_perpendicular_passed(status s);
	void set_arrival_circle_radius(double t);
	void set_waypoint_id(const waypoint & id) { waypoint_id = id; }
};
}
}

#endif
