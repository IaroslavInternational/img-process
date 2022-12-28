#include "App.hpp"

#include <fstream>
#include <algorithm>
#include <iterator>
#include <random>

namespace imglib
{
	/*****************************************************/

	Pixel::Pixel()
		:
		red(0.0f),
		green(0.0f),
		blue(0.0f)
	{}

	Pixel::Pixel(float r, float g, float b)
		: 
		red(r), 
		green(g), 
		blue(b)
	{}

	/*****************************************************/

	Image::Image(const std::string& filename)
		:
		filename(filename)
	{
		load();
	}

	void Image::load()
	{
		std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);

		for (int i = 0; i < pixels.size(); i++)
		{
			pixels[i].clear();
		}

		pixels.clear();

		if (file.fail())
		{
			throw "File error!";
		}
		else
		{
			bmpfile_magic magic;
			file.read((char*)(&magic), sizeof(magic));

			if (magic.magic[0] != 'B' || magic.magic[1] != 'M')
			{
				throw "Format error!";
			}
			else
			{
				bmpfile_header header;
				file.read((char*)(&header), sizeof(header));

				bmpfile_dib_info dib_info;
				file.read((char*)(&dib_info), sizeof(dib_info));

				bool flip = true;
				if (dib_info.height < 0)
				{
					flip = false;
					dib_info.height = -dib_info.height;
				}

				file.seekg(header.bmp_offset);

				for (int row = 0; row < dib_info.height; row++)
				{
					std::vector<Pixel> row_data;

					for (int col = 0; col < dib_info.width; col++)
					{
						int blue  = file.get();
						int green = file.get();
						int red   = file.get();

						row_data.push_back(Pixel((float)red, (float)green, (float)blue));
					}

					file.seekg(dib_info.width % 4, std::ios::cur);

					if (flip)
					{
						pixels.emplace(pixels.begin(), row_data);
					}
					else
					{
						pixels.emplace_back(row_data);
					}
				}

				file.close();
			}
		}
	}

	void Image::save(const std::string& filename)
	{
		std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);

		if (file.fail())
		{
			throw "Error while saving the file!";

		}
		else if (!is_image())
		{
			throw "Error format!";
		}
		else
		{
			bmpfile_magic magic;
			magic.magic[0] = 'B';
			magic.magic[1] = 'M';
			
			file.write((char*)(&magic), sizeof(magic));
			
			bmpfile_header header = { 0 };
			header.bmp_offset = sizeof(bmpfile_magic) + sizeof(bmpfile_header) + sizeof(bmpfile_dib_info);
			header.file_size = header.bmp_offset + (pixels.size() * 3 + pixels[0].size() % 4) * pixels.size();
			
			file.write((char*)(&header), sizeof(header));
			
			bmpfile_dib_info dib_info = { 0 };
			dib_info.header_size = sizeof(bmpfile_dib_info);
			dib_info.width = pixels[0].size();
			dib_info.height = pixels.size();
			dib_info.num_planes = 1;
			dib_info.bits_per_pixel = 24;
			dib_info.compression = 0;
			dib_info.bmp_byte_size = 0;
			dib_info.hres = 2835;
			dib_info.vres = 2835;
			dib_info.num_colors = 0;
			dib_info.num_important_colors = 0;
			file.write((char*)(&dib_info), sizeof(dib_info));

			for (int row = pixels.size() - 1; row >= 0; row--)
			{
				std::vector<Pixel> row_data;
				std::copy(pixels[row].begin(), pixels[row].end(), std::back_inserter(row_data));

				for (int col = 0; col < row_data.size(); col++)
				{
					const Pixel& pix = row_data[col];

					file.put((uchar_t)(pix.blue));
					file.put((uchar_t)(pix.green));
					file.put((uchar_t)(pix.red));
				}

				for (int i = 0; i < row_data.size() % 4; i++)
				{
					file.put(0);
				}
			}

			file.close();
		}
	}

	bool Image::is_image()
	{
		const int height = pixels.size();

		if (height == 0 || pixels[0].size() == 0)
		{
			return false;
		}

		const int width = pixels[0].size();

		for (int row = 0; row < height; row++)
		{
			if (pixels[row].size() != width)
			{
				return false;
			}

			for (int column = 0; column < width; column++)
			{
				Pixel current = pixels[row][column];

				if (current.red > MAX_RGB   || current.red < MIN_RGB   ||
					current.green > MAX_RGB || current.green < MIN_RGB ||
					current.blue > MAX_RGB  || current.blue < MIN_RGB)

					return false;
			}
		}
		return true;
	}

	void Image::from_pixel_matrix(const PixelMatrix& mtx)
	{
		pixels = mtx;
	}

	void Image::attach2obj(HWND hwnd, CStatic* obj)
	{
		CRect rect;
		obj->GetClientRect(rect);

		CClientDC dc(obj);

		CImage img;
		img.Load(str2wstring(filename).c_str());

		CBitmap c_bmp;
		c_bmp.Attach(img.Detach());

		CDC memoryDC;
		memoryDC.CreateCompatibleDC(&dc);
		memoryDC.SelectObject(c_bmp);

		BITMAP bmp;
		c_bmp.GetBitmap(&bmp);

		dc.SetStretchBltMode(COLORONCOLOR);
		dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memoryDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	}
	
	PixelMatrix Image::get_matrix()
	{
		if (is_image())
		{
			return pixels;
		}
		else
		{
			return PixelMatrix();
		}
	}

	size_t Image::get_width() const noexcept
	{
		return pixels[0].size();
	}

	size_t Image::get_height() const noexcept
	{
		return pixels.size();
	}

	void Image::set_filename(const std::string& filename)
	{
		this->filename = filename;
	}

	float* Image::get_R(int x, int y)
	{
		return &pixels[y][x].red;
	}

	float* Image::get_G(int x, int y)
	{
		return &pixels[y][x].green;
	}

	float* Image::get_B(int x, int y)
	{
		return &pixels[y][x].blue;
	}

	Pixel* Image::get_pixel(int x, int y)
	{
		return &pixels[y][x];
	}
	
	/*****************************************************/

	std::wstring str2wstring(const std::string& str)
	{
		std::wstring_convert<convert_t, wchar_t> strconverter;
		return strconverter.from_bytes(str);
	}

	int generate_rnd_number(int low, int high)
	{
		std::random_device rd;
		std::mt19937 rnd(rd());

		std::uniform_int_distribution<int> uid(low, high);

		return uid(rd);
	}

	void set_pixel(Pixel* p_pixel_component, int value)
	{
		p_pixel_component->red   = (float)value;
		p_pixel_component->green = (float)value;
		p_pixel_component->blue  = (float)value;
	}

	void to_blackwhite(Image& img, int threshold)
	{
		size_t width  = img.get_width();
		size_t height = img.get_height();

		float f;

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				f = (*img.get_R(i, j) + *img.get_G(i, j) + *img.get_B(i, j)) / 3.0f;
				
				if (f <= (float)threshold)
				{
					set_pixel(img.get_pixel(i, j), 0);
				}
				else
				{
					set_pixel(img.get_pixel(i, j), 255);
				}
			}
		}

		img.save("..\\res\\bw.bmp");
		img.set_filename("..\\res\\bw.bmp");
	}

	void to_monochrome(Image& img)
	{
		size_t width  = img.get_width();
		size_t height = img.get_height();

		float f;

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				f = (*img.get_R(i, j) + *img.get_G(i, j) + *img.get_B(i, j)) / 3;
				
				set_pixel(img.get_pixel(i, j), int(f));
			}
		}

		img.save("..\\res\\mono.bmp");
		img.set_filename("..\\res\\mono.bmp");
	}

	void add_noice(Image& img, int k)
	{
		k = 255 - k;

		size_t width  = img.get_width();
		size_t height = img.get_height();

		float f;

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				int rnd = generate_rnd_number(-255 + k, 255 - k);

				if (*img.get_R(i, j) + rnd < 255.0f && *img.get_R(i, j) + rnd > 0.0f)
				{
					*img.get_R(i, j) += rnd;
				}
				else if (*img.get_G(i, j) + rnd < 255.0f && *img.get_G(i, j) + rnd > 0.0f)
				{
					*img.get_G(i, j) += rnd;
				}
				else if (*img.get_B(i, j) + rnd < 255.0f && *img.get_B(i, j) + rnd > 0.0f)
				{
					*img.get_B(i, j) += rnd;
				}
			}
		}

		img.save("..\\res\\noise.bmp");
		img.set_filename("..\\res\\noise.bmp");
	}

	/*****************************************************/
}