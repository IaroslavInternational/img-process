#pragma once
#pragma warning(disable : 4996)

#include "Core.hpp"

#include <afxwin.h>
#include <atlimage.h>
#include <string>
#include <vector>
#include <codecvt>

namespace imglib
{
	/*****************************************************/

	struct Pixel;
	using  uchar_t	  = unsigned char;
	using  uint32_t	  = unsigned int;
	using  uint16_t	  = unsigned short int;
	using  int32_t	  = signed int;
	using  int16_t	  = signed short int;
	using  convert_t   = std::codecvt_utf8<wchar_t>;
	using  PixelMatrix = std::vector<std::vector<Pixel>>;
	
	/*****************************************************/

	constexpr int MIN_RGB = 0;
	constexpr int MAX_RGB = 255;
	constexpr int BMP_MAGIC_ID = 2;

	/*****************************************************/

	std::wstring IMGLIB_API str2wstring(const std::string& str);

	struct IMGLIB_API Pixel
	{
		Pixel();
		Pixel(int r, int g, int b);
		
		int red;
		int green;
		int blue;
	};

	class IMGLIB_API Image
	{
	public:
		Image(const std::string& filename);
    public:
        void		load();
        void		save(const std::string& filename);
        bool		is_image();
        void		from_pixel_matrix(const PixelMatrix& mtx);
		void		attach2obj(HWND hwnd, CStatic* obj);
		PixelMatrix get_matrix();
	public:
		int*   get_R(int x, int y);
		int*   get_G(int x, int y);
		int*   get_B(int x, int y);
		size_t get_width()  const noexcept;
		size_t get_height() const noexcept;
		void   set_filename(const std::string& filename);
    private:
		struct bmpfile_magic
		{
			uchar_t magic[BMP_MAGIC_ID];
		};

		struct bmpfile_header
		{
			uint32_t file_size;
			uint16_t creator1;
			uint16_t creator2;
			uint32_t bmp_offset;
		};

		struct bmpfile_dib_info
		{
			uint32_t header_size;
			int32_t  width;
			int32_t  height;
			uint16_t num_planes;
			uint16_t bits_per_pixel;
			uint32_t compression;
			uint32_t bmp_byte_size;
			int32_t  hres;
			int32_t  vres;
			uint32_t num_colors;
			uint32_t num_important_colors;
		};
    private:
        PixelMatrix pixels;
		std::string filename;
	};

	void IMGLIB_API to_monochrome(Image& img);

	/*****************************************************/
}