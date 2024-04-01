#include <bit>
#include <tuple>
#include <array>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

#include <cstdint>

template <typename T>
class BitIterator{
	public:
		explicit BitIterator(const T m): m_mask{m}{
		}

		inline BitIterator begin()const{
			return BitIterator{m_mask};
		}

		inline BitIterator end()const{
			return BitIterator{0x0};
		}

		inline void operator++ (){
			m_mask &= m_mask - 1;
		}

		inline uint8_t operator* ()const{
			return std::countr_zero(m_mask);
		}

		inline friend bool operator== (const BitIterator a, const BitIterator b){
			return a.m_mask == b.m_mask;
		}

		inline friend bool operator!= (const BitIterator a, const BitIterator b){
			return !(a == b);
		}

	private:
		T m_mask;
};

enum class Color: uint8_t{
	WHITE,   BLUE,    RED,
	GREEN, ORANGE, YELLOW,

	NONE
};

static constexpr const Color COLORS[] = {
	Color::WHITE, Color::  BLUE, Color::   RED,
	Color::GREEN, Color::ORANGE, Color::YELLOW,
};

static constexpr const char color_to_char_table[] = {
	'w', 'b', 'r',
	'g', 'o', 'y',

	' '
};

static inline char color_to_char(const Color color){
	return color_to_char_table[static_cast<uint8_t>(color)];
}

// horizonal and vertical
static constexpr const uint64_t adj_fields[] = {
	0x0001000003000000, 0x0001000006000000, 0x000100000c000000,
	0x0001000009000000, 0x0002000030000000, 0x0002000060000000,
	0x00020000c0000000, 0x0002000090000000, 0x0004000300000000,
	0x0004000600000000, 0x0004000c00000000, 0x0004000900000000,
	0x0008003000000000, 0x0008006000000000, 0x000800c000000000,
	0x0008009000000000, 0x0010030000000000, 0x0010060000000000,
	0x00100c0000000000, 0x0010090000000000, 0x0020300000000000,
	0x0020600000000000, 0x0020c00000000000, 0x0020900000000000,
	0x0000000000000009, 0x0000000000000003, 0x0000000000000006,
	0x000000000000000c, 0x0000000000000090, 0x0000000000000030,
	0x0000000000000060, 0x00000000000000c0, 0x0000000000000900,
	0x0000000000000300, 0x0000000000000600, 0x0000000000000c00,
	0x0000000000009000, 0x0000000000003000, 0x0000000000006000,
	0x000000000000c000, 0x0000000000090000, 0x0000000000030000,
	0x0000000000060000, 0x00000000000c0000, 0x0000000000900000,
	0x0000000000300000, 0x0000000000600000, 0x0000000000c00000,
};

// horizonal and vertical and diagonal
//static constexpr const uint64_t adj_fields[] = {
//	0x000100000300000a, 0x0001000006000005, 0x000100000c00000a,
//	0x0001000009000005, 0x00020000300000a0, 0x0002000060000050,
//	0x00020000c00000a0, 0x0002000090000050, 0x0004000300000a00,
//	0x0004000600000500, 0x0004000c00000a00, 0x0004000900000500,
//	0x000800300000a000, 0x0008006000005000, 0x000800c00000a000,
//	0x0008009000005000, 0x00100300000a0000, 0x0010060000050000,
//	0x00100c00000a0000, 0x0010090000050000, 0x0020300000a00000,
//	0x0020600000500000, 0x0020c00000a00000, 0x0020900000500000,
//	0x0001000000000009, 0x0001000000000003, 0x0001000000000006,
//	0x000100000000000c, 0x0002000000000090, 0x0002000000000030,
//	0x0002000000000060, 0x00020000000000c0, 0x0004000000000900,
//	0x0004000000000300, 0x0004000000000600, 0x0004000000000c00,
//	0x0008000000009000, 0x0008000000003000, 0x0008000000006000,
//	0x000800000000c000, 0x0010000000090000, 0x0010000000030000,
//	0x0010000000060000, 0x00100000000c0000, 0x0020000000900000,
//	0x0020000000300000, 0x0020000000600000, 0x0020000000c00000,
//};

// horizontal and vertical and diagonal (also over multiple faces)
//static constexpr const uint64_t adj_fields[] = {
//	0x000100003300000a, 0x0001000306000005, 0x000100300c00000a,
//	0x0001030009000005, 0x00020000330000a0, 0x0002090060000050,
//	0x00023000c00000a0, 0x0002000690000050, 0x0004000306000a00,
//	0x0004000690000500, 0x0004600c00000a00, 0x0004006900000500,
//	0x000800300c00a000, 0x0008006900005000, 0x0008c0c00000a000,
//	0x0008069000005000, 0x00100300090a0000, 0x0010069000050000,
//	0x00109c00000a0000, 0x0010090060050000, 0x00203000c0a00000,
//	0x0020600c00500000, 0x0020c0c000a00000, 0x00209c0000500000,
//	0x0001000000010019, 0x0001000000000113, 0x0001000000001106,
//	0x000100000001100c, 0x0002000000000291, 0x0002000000080031,
//	0x0002000000180060, 0x00020000001002c0, 0x0004000000002902,
//	0x0004000000000382, 0x0004000000200680, 0x0004000000202c00,
//	0x0008000000029004, 0x0008000000003804, 0x0008000000406800,
//	0x000800000042c000, 0x0010000000090028, 0x0010000000038008,
//	0x0010000000868000, 0x00100000008c0020, 0x0020000000940040,
//	0x0020000000300440, 0x0020000000604400, 0x0020000000c44000,
//};

static constexpr const uint64_t faces_mask[] = {
	0x000100000f00000f, 0x00020000f00000f0,
	0x0004000f00000f00, 0x000800f00000f000,
	0x00100f00000f0000, 0x0020f00000f00000,
};

static constexpr const std::tuple<uint8_t, uint8_t> edge_to_fields[12] = {
	{ 0,  4}, { 1,  8}, { 7,  9},
	{ 5, 19}, { 3, 16}, { 2, 12},
	{15, 17}, {11, 13}, { 6, 20},
	{10, 21}, {14, 22}, {18, 23},
};

static constexpr const std::tuple<uint8_t, uint8_t, uint8_t> corner_to_fields[8] = {
	{25, 28, 33}, {24, 40, 29},
	{27, 36, 41}, {26, 32, 37},
	{30, 43, 44}, {42, 39, 47},
	{38, 35, 46}, {34, 31, 45},
};

static constexpr const std::tuple<Color, Color> edge_color_options[12] = {
	{Color:: WHITE, Color::  BLUE}, {Color:: WHITE, Color::   RED},
	{Color::  BLUE, Color::   RED}, {Color::  BLUE, Color::ORANGE},
	{Color:: WHITE, Color::ORANGE}, {Color:: WHITE, Color:: GREEN},
	{Color:: GREEN, Color::ORANGE}, {Color::   RED, Color:: GREEN},
	{Color::  BLUE, Color::YELLOW}, {Color::   RED, Color::YELLOW},
	{Color:: GREEN, Color::YELLOW}, {Color::ORANGE, Color::YELLOW},
};

static constexpr const std::tuple<Color, Color, Color> corner_color_options[8] = {
	{Color:: WHITE, Color::  BLUE, Color::   RED},
	{Color:: WHITE, Color::ORANGE, Color::  BLUE},
	{Color:: WHITE, Color:: GREEN, Color::ORANGE},
	{Color:: WHITE, Color::   RED, Color:: GREEN},
	{Color::  BLUE, Color::ORANGE, Color::YELLOW},
	{Color::ORANGE, Color:: GREEN, Color::YELLOW},
	{Color:: GREEN, Color::   RED, Color::YELLOW},
	{Color::   RED, Color::  BLUE, Color::YELLOW},
};

static inline constexpr uint64_t square_to_mask(const uint8_t square){
	return static_cast<uint64_t>(0x1) << square;
}

static inline constexpr uint64_t color_to_index(const Color color){
	return static_cast<uint8_t>(color);
}

static inline constexpr uint8_t color_to_center_index(const Color color){
	return color_to_index(color) + 48;
}

static constexpr const std::array<uint64_t, 6> INIT_CUBE_COLORS = {
	static_cast<uint64_t>(0x1) << color_to_center_index(Color:: WHITE),
	static_cast<uint64_t>(0x1) << color_to_center_index(Color::  BLUE),
	static_cast<uint64_t>(0x1) << color_to_center_index(Color::   RED),
	static_cast<uint64_t>(0x1) << color_to_center_index(Color:: GREEN),
	static_cast<uint64_t>(0x1) << color_to_center_index(Color::ORANGE),
	static_cast<uint64_t>(0x1) << color_to_center_index(Color::YELLOW),
};

class Cube{
	public:
		explicit Cube(): m_colors{INIT_CUBE_COLORS}{
		}

		inline void do_single_move(const uint8_t square, const Color color){
			m_colors[color_to_index(color)] |= square_to_mask(square);
		}

		inline void undo_single_move(const uint8_t square, const Color color){
			m_colors[color_to_index(color)] &= ~square_to_mask(square);
		}

		inline void do_edge_move(
				const uint8_t sq0, const Color c0,
				const uint8_t sq1, const Color c1
			){

			do_single_move(sq0, c0);
			do_single_move(sq1, c1);
		}

		inline void undo_edge_move(
				const uint8_t sq0, const Color c0,
				const uint8_t sq1, const Color c1
			){

			undo_single_move(sq0, c0);
			undo_single_move(sq1, c1);
		}

		inline void do_corner_move(
				const uint8_t sq0, const Color c0,
				const uint8_t sq1, const Color c1,
				const uint8_t sq2, const Color c2
			){

			do_single_move(sq0, c0);
			do_single_move(sq1, c1);
			do_single_move(sq2, c2);
		}

		inline void undo_corner_move(
				const uint8_t sq0, const Color c0,
				const uint8_t sq1, const Color c1,
				const uint8_t sq2, const Color c2
			){

			undo_single_move(sq0, c0);
			undo_single_move(sq1, c1);
			undo_single_move(sq2, c2);
		}

		inline bool is_valid_around_square(const uint8_t square, const Color color)const{
			return 0 == (m_colors[color_to_index(color)] & adj_fields[square]);
		}

		inline bool is_valid_around_edge(
				const uint8_t sq0, const Color c0,
				const uint8_t sq1, const Color c1
			)const{

			return is_valid_around_square(sq0, c0)
				&& is_valid_around_square(sq1, c1);
		}

		inline bool is_valid_around_corner(
				const uint8_t sq0, const Color c0,
				const uint8_t sq1, const Color c1,
				const uint8_t sq2, const Color c2
			)const{

			return is_valid_around_square(sq0, c0)
				&& is_valid_around_square(sq1, c1)
				&& is_valid_around_square(sq2, c2);
		}

		inline bool every_color_on_every_face_at_least_once()const{
			for(uint8_t c = 0; c < 6; ++c){
				for(uint8_t f = 0; f < 6; ++f){
					if(std::popcount(m_colors[c] & faces_mask[f]) < 1)
						return false;
				}
			}

			return true;
		}

		// Note: This methode is slow.
		inline Color get(const uint8_t idx)const{
			const uint64_t mask = static_cast<uint64_t>(0x1) << idx;

			for(const Color color : COLORS){
				if(0 != (get_color_bitfield(color) & mask))
					return color;
			}

			return Color::NONE;
		}

		void serialize(std::ostream& os)const{
			std::ostringstream oss{};
			for(uint8_t i = 0; i < 6 * 9; ++i)
				oss << color_to_char(get(i));

			os << oss.str();
		}

		friend std::ostream& operator<< (std::ostream& os, const Cube& cube){
			os
				<< "            +---+---+---+\n"
				<< "            | " << color_to_char(cube.get(30)) << " | " << color_to_char(cube.get(6)) << " | " << color_to_char(cube.get(31)) << " |\n"
				<< "            +---+---+---+\n"
				<< "            | " << color_to_char(cube.get(5)) << " | " << color_to_char(cube.get(49)) << " | " << color_to_char(cube.get(7)) << " |\n"
				<< "            +---+---+---+\n"
				<< "            | " << color_to_char(cube.get(29)) << " | " << color_to_char(cube.get(4)) << " | " << color_to_char(cube.get(28)) << " |\n"
				<< "+---+---+---+===+===+===+---+---+---+\n"
				<< "| " << color_to_char(cube.get(43)) << " | " << color_to_char(cube.get(19)) << " | " << color_to_char(cube.get(40)) << " | " << color_to_char(cube.get(24)) << " | " << color_to_char(cube.get(0)) << " | " << color_to_char(cube.get(25)) << " | " << color_to_char(cube.get(33)) << " | " << color_to_char(cube.get(9)) << " | " << color_to_char(cube.get(34)) << " |\n"
				<< "+---+---+---+---+---+---+---+---+---+\n"
				<< "| " << color_to_char(cube.get(18)) << " | " << color_to_char(cube.get(52)) << " | " << color_to_char(cube.get(16)) << " | " << color_to_char(cube.get(3)) << " | " << color_to_char(cube.get(48)) << " | " << color_to_char(cube.get(1)) << " | " << color_to_char(cube.get(8)) << " | " << color_to_char(cube.get(50)) << " | " << color_to_char(cube.get(10)) << " |\n"
				<< "+---+---+---+---+---+---+---+---+---+\n"
				<< "| " << color_to_char(cube.get(42)) << " | " << color_to_char(cube.get(17)) << " | " << color_to_char(cube.get(41)) << " | " << color_to_char(cube.get(27)) << " | " << color_to_char(cube.get(2)) << " | " << color_to_char(cube.get(26)) << " | " << color_to_char(cube.get(32)) << " | " << color_to_char(cube.get(11)) << " | " << color_to_char(cube.get(35)) << " |\n"
				<< "+---+---+---+===+===+===+---+---+---+\n"
				<< "            | " << color_to_char(cube.get(36)) << " | " << color_to_char(cube.get(12)) << " | " << color_to_char(cube.get(37)) << " |\n"
				<< "            +---+---+---+\n"
				<< "            | " << color_to_char(cube.get(15)) << " | " << color_to_char(cube.get(51)) << " | " << color_to_char(cube.get(13)) << " |\n"
				<< "            +---+---+---+\n"
				<< "            | " << color_to_char(cube.get(39)) << " | " << color_to_char(cube.get(14)) << " | " << color_to_char(cube.get(38)) << " |\n"
				<< "            +===+===+===+\n"
				<< "            | " << color_to_char(cube.get(47)) << " | " << color_to_char(cube.get(22)) << " | " << color_to_char(cube.get(46)) << " |\n"
				<< "            +---+---+---+\n"
				<< "            | " << color_to_char(cube.get(23)) << " | " << color_to_char(cube.get(53)) << " | " << color_to_char(cube.get(21)) << " |\n"
				<< "            +---+---+---+\n"
				<< "            | " << color_to_char(cube.get(44)) << " | " << color_to_char(cube.get(20)) << " | " << color_to_char(cube.get(45)) << " |\n"
				<< "            +---+---+---+\n"
				<< '\n';

			cube.serialize(os);
			return os << '\n';
		}

	private:
		inline uint64_t get_color_bitfield(const Color color)const{
			return m_colors[static_cast<uint8_t>(color)];
		}

		std::array<uint64_t, 6> m_colors;
};

// Note: Optimal ordering for maximal overlap.
// (index) |-> (edge/square_index, is_edge)

// horizonal and vertical
static constexpr const std::pair<uint8_t, bool> block_search_order[20] = {
	{ 0, true}, {1, true}, {2, true}, {0, false},
	{ 3, true}, {4, true},            {1, false},
	{ 5, true}, {6, true},            {2, false},
	{ 7, true},                       {3, false},
	{ 8, true}, {9, true},            {7, false},
	{10, true},                       {6, false},
	{11, true},                       {4, false}, {5, false},
};

// horizonal and vertical and diagonal
//static constexpr const std::pair<uint8_t, bool> block_search_order[20] = {
//	{0, true}, {1, true}, {2, true},
//	{3, true}, {6, true}, {4, true},
//	{7, true}, {5, true},
//	{8, true}, {9, true}, {10, true}, {11, true},
//	{0, false}, {1, false}, {2, false}, {3, false},
//	{4, false}, {5, false}, {6, false}, {7, false},
//};

// horizontal and vertical and diagonal (also over multiple faces)
//static constexpr const std::pair<uint8_t, bool> block_search_order[20] = {
//	{ 0, true}, {1, true}, {2, true}, {0, false},
//	{ 3, true}, {4, true},            {1, false},
//	{ 5, true}, {6, true},            {2, false},
//	{ 7, true},                       {3, false},
//	{ 8, true}, {9, true},            {7, false},
//	{10, true},                       {6, false},
//	{11, true},                       {4, false}, {5, false},
//};

static constexpr const uint16_t INIT_AVAILABLE_EDGE_COLORS = 0xFFF;
static constexpr const uint16_t INIT_AVAILABLE_CORNER_COLORS = 0xFF;

static uint64_t msg_count = 0;
static uint64_t total_count = 0;
static std::chrono::high_resolution_clock::time_point start{};

template <uint8_t depth>
static void search(
		Cube& cube,
		const uint16_t available_edge_colors = INIT_AVAILABLE_EDGE_COLORS,
		const uint8_t available_corner_colors = INIT_AVAILABLE_CORNER_COLORS
	){

	if constexpr(20 == depth){
		if(true /*cube.every_color_on_every_face_at_least_once()*/){
			total_count += 1;
			if(total_count % 0x8000000 == 0){
				++msg_count;
			
				const auto now = std::chrono::high_resolution_clock::now();
				const double dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
			
				std::ostringstream oss{};
				oss << std::setprecision(2) << std::fixed
					<< '[' << std::setw(8) << msg_count << "]: "
					<< "N: " << std::setw(8) << double(total_count) / 1'000'000'000 << " Bio.;\t"
					<< std::setprecision(3)
					<< "D: " << std::setw(5) << dt / (1000.0 * 60.0 * 60.0) <<  " h;\t"
					<< std::setprecision(2)
					<< "S: " << std::setw(3) << ((1'000.0 * total_count) / dt) / 1'000'000.0 << " MCps\n";
			
				std::cout << oss.str();
			}

			// TODO: check if cube configuration is reachable from start position (parity).
			// cube.serialize(std::cout << '\n');
			// std::cout << cube << '\n';
		}
	}else{
		const auto[index, is_edge] = block_search_order[depth];
		if(is_edge){
			const auto[sq0, sq1] = edge_to_fields[index];

			for(const uint8_t edge_color_option_idx : BitIterator{available_edge_colors}){
				const auto[c0, c1] = edge_color_options[edge_color_option_idx];

				cube.do_edge_move(sq0, c0, sq1, c1);

				if(cube.is_valid_around_edge(sq0, c0, sq1, c1)){
					const uint16_t new_available_edge_colors =
						available_edge_colors & ~(static_cast<uint16_t>(0x1) << edge_color_option_idx);

					search<depth + 1>(
						cube,
						new_available_edge_colors,
						available_corner_colors
					);
				}

				cube.undo_edge_move(sq0, c0, sq1, c1);

				cube.do_edge_move(sq0, c1, sq1, c0);

				if(cube.is_valid_around_edge(sq0, c1, sq1, c0)){
					const uint16_t new_available_edge_colors =
						available_edge_colors & ~(static_cast<uint16_t>(0x1) << edge_color_option_idx);

					search<depth + 1>(
						cube,
						new_available_edge_colors,
						available_corner_colors
					);
				}

				cube.undo_edge_move(sq0, c1, sq1, c0);
			}
		}else{ // !is_edge
			const auto[sq0, sq1, sq2] = corner_to_fields[index];

			for(const uint8_t corner_color_option_idx : BitIterator{available_corner_colors}){
				const auto[c0, c1, c2] = corner_color_options[corner_color_option_idx];

				cube.do_corner_move(sq0, c0, sq1, c1, sq2, c2);

				if(cube.is_valid_around_corner(sq0, c0, sq1, c1, sq2, c2)){
					const uint16_t new_available_corner_colors =
						available_corner_colors & ~(static_cast<uint8_t>(0x1) << corner_color_option_idx);

					search<depth + 1>(
						cube,
						available_edge_colors,
						new_available_corner_colors
					);
				}

				cube.undo_corner_move(sq0, c0, sq1, c1, sq2, c2);

				cube.do_corner_move(sq0, c2, sq1, c0, sq2, c1);

				if(cube.is_valid_around_corner(sq0, c2, sq1, c0, sq2, c1)){
					const uint16_t new_available_corner_colors =
						available_corner_colors & ~(static_cast<uint8_t>(0x1) << corner_color_option_idx);

					search<depth + 1>(
						cube,
						available_edge_colors,
						new_available_corner_colors
					);
				}

				cube.undo_corner_move(sq0, c2, sq1, c0, sq2, c1);

				cube.do_corner_move(sq0, c1, sq1, c2, sq2, c0);

				if(cube.is_valid_around_corner(sq0, c1, sq1, c2, sq2, c0)){
					const uint16_t new_available_corner_colors =
						available_corner_colors & ~(static_cast<uint8_t>(0x1) << corner_color_option_idx);

					search<depth + 1>(
						cube,
						available_edge_colors,
						new_available_corner_colors
					);
				}

				cube.undo_corner_move(sq0, c1, sq1, c2, sq2, c0);
			}
		}
	}
}

int main(){
	Cube init_cube{};

	msg_count = 0;
	total_count = 0;
	start = std::chrono::high_resolution_clock::now();

	search<0>(init_cube);

	std::cout << "\ndone: " << total_count << '\n';
	return 0;
}
