Play nhạc nền : 
wchar_t* test = TEXT("E:/b.wav");
				PlayAudio(test);
Play nhạc khác :
	doMCICommandStringStop(hWnd);
				wchar_t* test = TEXT("E:/a.wav");
				doMCICommandStringPlay(hWnd, test);
Lớp Data: 
	- Dùng để truy xuất lấy dữ liệu từ file.
		Format file :  Mỗi dòng là một tập : Key <khoảng trắng> Đường dẫn
	Đầu tiên Init() để load dữ liệu từ file.
	Muốn lấy đường dẫn của key a thì gọi hàm GetLink(a) . Dữ liệu trả về là kiểu std::wstring