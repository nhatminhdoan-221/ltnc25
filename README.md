# ltnc25

1. Thông tin về game: link (github, google doc, youtube), game tham khảo (link, miêu tả) 
- Github: https://github.com/nhatminhdoan-221/ltnc25
- Google Drive (văn bản, video): https://drive.google.com/drive/u/0/folders/15-rrZRJno-tgFb_btKELxItqQE4YJHHu
- Game tham khảo: Tetris (của thầy Khôi)

2. Các tính năng cải tiến trong game
- Sử dụng Menu có 2 phương án để lựa chọn các Options trong Menu. Có thể sử dụng chuột và click vào các nhu cầu (Starts Game, Quit, Sound: On/Off). Hoặc có thể sử dụng các nút bấm Lên/Xuống/SpaceBar/Enter để thực hiện chức năng dù không có chuột để click tại Menu. 
- Trong quá trình chơi game, có thể điều chỉnh hình dáng của tetromino bằng phím Z hoặc phím UP (lên). Có thể bấm phím M, để tắt/bật âm thanh. Có một chiếc đồng hồ để đến thời gian đang chơi được của người chơi ở phía bên trái bảng. Bên phải của bảng chơi, cập nhật điểm số, level, số hàng phá được trong level đó. 
- Cách tính điểm: Nếu người chơi phá được 1 đến 4 hàng, số điểm lần lượt là 10, 30, 60, 100. Số điểm được cộng vào điểm số tích lũy có công thức: 
	Số điểm được cộng = Điểm số hàng phá được * (Level của người chơi + 9) / 10
- Có tối đa 20 levels để người chơi có thể nâng cao điểm số.
- Tốc độ của các tetromino sẽ tăng dần theo thời gian. 
- Khi Game Over, kết quả liên quan đến điểm số, level, số hàng đã phá được sẽ hiện lên để thông báo cho người chơi.Người chơi có thể bấm Restart để về trang chính hoặc Quit để dừng tham gia trò chơi.

3. Cấu trúc của project game: Tổ chức lớp, Miêu tả luồng, …
- Header Files
	board.h (gọi tên các hàm, biến sử dụng trong bảng chơi)
	constants.h (lưu lại các giá trị về cửa sổ, hằng số các thành phần trong bảng)
	game.h (gọi tên các hàm, biến để quản lí và vận hành game: font, âm thanh,....)
	GameUI.h (gọi tên các hàm để render Game Over và chữ hiện lên trong quá trình chơi)
	pieces.h (lưu lại cấu tạo các mảnh trong vùng 4x4, vùng chiếm trong không gian của từng mảnh theo cách quay)
	queue.h (gọi tên các hàm, biến để tạo ra tetromino trong game)
	tetromino.h (gọi tên các hàm, biến liên quan đến chuyển động, vị trí của tetromino trong bảng)
- Source Files
	board.cpp
	game.cpp
	GameUI.cpp
	main.cpp
	queue.cpp
	tetromino.cpp

4. Nguồn tham khảo và lấy dữ liệu: âm thanh, đồ họa, chatgpt, 
- Âm thanh: (Youtube): https://www.youtube.com/watch?v=Ci5squuWW3Q
- Hình ảnh: Hình được vẽ bởi ChatGPT
- Font chữ: Press_Start_2P

