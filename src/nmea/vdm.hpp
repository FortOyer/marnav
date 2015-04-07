#ifndef __NMEA__VDM__HPP__
#define __NMEA__VDM__HPP__

#include "sentence.hpp"
#include <utils/optional.hpp>

namespace marnav
{
namespace nmea
{

class vdm : public sentence
{
public:
	constexpr static const sentence_id ID = sentence_id::VDM;

	vdm();
	vdm(const vdm &) = default;
	vdm & operator=(const vdm &) = default;

	static std::unique_ptr<sentence> parse(const std::string & talker,
		const std::vector<std::string> & fields) throw(std::invalid_argument);

protected:
	vdm(sentence_id id, const std::string & tag, const std::string & talker);

	virtual std::vector<std::string> get_data() const override;
	virtual char get_start_token() const override { return START_TOKEN_AIS; }

	void read_fields(const std::vector<std::string> & fields);

private:
	uint32_t n_fragments;
	uint32_t fragment;
	utils::optional<uint32_t> seq_msg_id;
	char radio_channel; // A = 161.975MHz (87B), B = 162.025MHz (88B)
	std::string payload; // 6bit encoded content
	uint32_t n_fill_bits; // 0..5

public:
	decltype(n_fragments) get_n_fragments() const { return n_fragments; }
	decltype(fragment) get_fragment() const { return fragment; }
	decltype(seq_msg_id) get_seq_msg_id() const { return seq_msg_id; }
	decltype(radio_channel) get_radio_channel() const { return radio_channel; }
	decltype(payload) get_payload() const { return payload; }
	decltype(n_fill_bits) get_n_fill_bits() const { return n_fill_bits; }

	void set_n_fragments(uint32_t t) { n_fragments = t; }
	void set_fragment(uint32_t t) { fragment = t; }
	void set_seq_msg_id(uint32_t t) { seq_msg_id = t; }
	void set_radio_channel(char channel) { radio_channel = channel; }
	void set_payload(const std::string & data, uint32_t fill_bits)
	{
		payload = data;
		n_fill_bits = fill_bits;
	}
	void set_payload(const std::pair<std::string, int> & data)
	{
		set_payload(data.first, data.second);
	}
};

/// Collects payload from proper NMEA sentences.
///
/// @note This function assumes, that all sentences in the specified range are
///   VDM or descendents, providing payload.
/// @note This function assumes, that all sentences in the range are pointers to sentences.
///
/// @param[in] begin Iterator pointing to the beginning of the messges to process.
/// @param[in] end   Iterator pointing after the messages to process (will not be
///   processed).
/// @return The container with all payload and padding bit information.
///
/// @todo Exetend/overload this function to accept containers of objects.
template <class InputIt>
std::vector<std::pair<std::string, int>> collect_payload(InputIt begin, InputIt end)
{
	std::vector<std::pair<std::string, int>> v;
	v.reserve(distance(begin, end));

	for (; begin != end; ++begin) {
		const auto & s = sentence_cast<nmea::vdm>(*begin);
		v.push_back(make_pair(s->get_payload(), s->get_n_fill_bits()));
	}

	return v;
}
}
}

#endif
