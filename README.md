# TheLordOfSlime
- Đây là bài tập lớn chiếm 20% tổng điểm của học phần lập trình nâng cao của Trường đại học Công Nghệ,ĐHQGHN
- Sử dụng thư viện SDL2, CodeBlock và C++
- Sinh viên thực hiện:
+ Tên: Đỗ Quốc Nguyên Chương
+ Mã sinh viên: 24020050
- Game thực hiện: The lord of Slime:
  + Đây là tựa game lấy cảm hứng từ bộ anime "chuyển sinh thành slime". 
   // Nguồn: https://vi.wikipedia.org/wiki/V%E1%BB%81_chuy%E1%BB%87n_t%C3%B4i_chuy%E1%BB%83n_sinh_th%C3%A0nh_Slime
  + Logic game có sự tham khảo từ Dino in zungle và có sự hỗ trợ của chatGPT
   // Nguồn: https://github.com/thai-nm/dino-in-zungle
  + Mức độ sử dụng AI: +) Có sử dụng chatGPt trong việc chỉnh và hoàn thiện logic code
                       +) có sử dụng AI để giải thích công dụng của các câu lệnh trong SDL2 image và mixer
  + Âm thanh có lấy từ Dino in zungle và từ các trang web trên mạng
   // Nguồn: +) https://github.com/thai-nm/dino-in-zungle
             +) https://pixabay.com/vi/sound-effects/search/chi%E1%BA%BFn%20th%E1%BA%AFng/
             +) https://tiengdong.com/
  + Ảnh: +) Các nhân vật và đạn: tự vẽ
         +) Background: lấy từ trên mạng
              // Nguồn: https://www.remove.bg/t/change-background


(*) Các phiên bản và sự cải tiến:
- Ngày 11/03/2025: version 0.1: đây là phiên bản đơn sơ nhất( chỉ bao gồm nguyên lí hoạt động của game)
- Ngày 12/03/2025: version 0.2
  + Phiên bản này đã có cải tiến thêm file ttf để hiện lên màn hình phần game over
  + Thêm các logic game như: xóa đạn khi ra khỏi màn hình, xóa obstacle khi ra khỏi màn hình, đạn bị cản bởi obstacle
- Ngày 12/03/2025: version 0.3
  + Phiên bản này đã sửa được lỗi đơ lag màn hình khi chạy chương trình và các obstacle ra một cách tương đối hợp lí
- Ngày 19/03/2025: version 1.0
  + Đây là phiên bản hoàn thiện logic game đầu tiên( sửa xóa đạn và obs khi ra ngoài màn hình, thêm kiểm tra va chạm khi 2 đạn của boss và slime gặp nhau)
  + Ở phiên bản này đã thêm được texture cho các nhân vật(slime, obs, địch nhỏ, boss, đạn của boss và slime) và làm cho background cuộn theo sự di chuyển cho màn hình
- Ngày 20/03/2025: version 1.5
  + Phiên bản này có sự hoàn thiện hơn( đã có thêm menu với 3 mức độ easy,medium,hard) và thay đổi các chỉ số của các nhân vật theo từng chế độ
  + Phiên bản này cũng đã hoàn thiện sơ bộ được bộ nút restart và exit với chức năng(restart: quay lại màn hình menu và exit: thoát khỏi game)
- Ngày 21/03/2025: version 2.01
  + Đây là phiên bản có sự đầy đủ nhất (có thêm âm thanh khi chọn ở menu, khi nhảy, khi vào màn chơi, khi bấm chuột)
  + Phiên bản này còn có sự cải tiến các nút bấm khi ở màn menu và gameover
  + Ở phiên bản này còn có thêm tính năng pause khi người chơi nhấn phím "P"
- Ngày 21/03/2025: versiom 2.67
  + Phiên bản này có sự thêm và chỉnh sửa về âm thanh của đạn, va chạm với slime và game lose
 
(*)Sự khác biệt và điểm mong muốn:
 - Sự khác biệt:
    Mặc dù có sự tham khảo từ dino in zunggle nhưng logic game đã có sự thay đổi khá nhiều ví dụ:
     +) Ở Dino thì game sẽ chạy vô hạn và tính điểm còn ở game Slime thì lại có kết thúc khi boss bị đánh bại hoặc slime hết máu hoặc hết thời gian
   
     +) Ở game Slime này còn có 3 chế độ chơi để người chơi có thể chọn thoải mái(easy,medium,hard)
   
     +) Ở game Slime còn có bộ nút pause game khi người chơi ấn "p" và bộ nút gameover khi người chơi kết thúc ván đấu, cả 2 bộ nút đều có 2 nút là "restart" và "exit" với
    "restart" có chức năng quay lại menu để chọn bàn chơi và "exit" để thoát khỏi trò chơi
   
     +) Sự tham khảo của game Slime với game Dino chỉ dừng lại ở ý tưởng để back ground chạy liên tục, các logic game, dòng code và cách chia file gần như khác hoàn toàn
   
     +) Các nhân vật và đạn của nhân vật đều là sản phẩm do chính tay em tự vẽ, chỉ có background và âm thanh là tham khảo từ các nguồn trên mạng
   
 - Điểm mong muốn:
    Từ các điểm đặc biệt em đã nêu ở phần khác biệt, em tin rằng game của em có thể đạt được mức điểm tối đa (điểm 10)
