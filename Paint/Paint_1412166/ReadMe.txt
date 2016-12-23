******************************************
*Họ và tên : Nguyễn Văn Hiếu
*MSSV      : 1412166
*Các yêu cầu đã hoàn thành: 100%
*	+ Xây dựng được giao diện giống với Project yêu cầu: 
		- Menu gồm có File và Draw có xử lý phím tắt
		- Toolbar có xử lý các sự kiện nhấn, thể hiện hình vẽ đang chọn hiện tại
		- Vùng client 
		- Thanh Statusbar
	+ Hoàn thành các yêu cầu chính của Project:
		- Khởi đầu với các phím Draw và Save bị disable.
		- Chức năng New:
			Xóa cửa sổ Editor. Đặt tên file mặc định là Noname.mpt
		- Open: 
			Load một file .mpt có sẵn.
		- Save: 
			Lưu lại file hiện hành. Nếu là file mới thì dialog save as để nhập tên lưu lại.
			Nếu là file đã lưu hoặc file đã có thì lưu lại trạng thái hiện tại.
		- Color : 
			Mở hộp thoại Color cho phép chọn màu
		- Line:
			Cho phép chọn chế độ vẽ đường thẳng.
		- Rectange:
			Cho phép chọn chế độ vẽ hình chữ nhật
		- Ellipse: 
			Cho phép chọn chế độ vẽ hình Ellipse
		- Xử lý được sự kiện WM_PAINT
		- Xử lý được sự kiện lưu file và load file
	+ Format file *.mpt
		Dòng 1: Số lượng hình vẽ
		Dòng 2 ... n: Các hình vẽ được lưu theo cấu trúc (x1 y1 x2 y2 color shape)
*Các yêu cầu chưa hoàn thành: 0%
*Version: Visual Studio 2015
*Lưu ý: Khi build sử dụng chế độ build x86. 
		Add file ComCtl32.Lib trong folder ../x86