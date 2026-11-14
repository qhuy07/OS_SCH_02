# Báo cáo đề tài OS_SCH_02
## SRTF và SJF không độc quyền

## 1. Giới thiệu
Lập lịch CPU là chức năng của hệ điều hành nhằm lựa chọn tiến trình trong hàng đợi sẵn sàng để cấp CPU. Đề tài xây dựng ứng dụng desktop mô phỏng và so sánh SJF không độc quyền với SRTF có độc quyền.

## 2. Mục tiêu
- Cài đặt hai thuật toán lập lịch CPU.
- Minh họa thứ tự thực thi bằng biểu đồ Gantt.
- Tính CT, TAT, WT và RT.
- Đọc dữ liệu từ CSV, kiểm tra dữ liệu đầu vào và xuất kết quả.
- So sánh kết quả trên cùng một bộ tiến trình.

## 3. Phạm vi và giả định
- Mỗi tiến trình có PID, Arrival Time và CPU Burst Time.
- Arrival Time là số nguyên không âm; Burst Time là số nguyên dương; PID không trùng.
- Thời gian chuyển ngữ cảnh được giả định bằng 0.
- Khi bằng nhau, chương trình dùng Arrival Time rồi PID để phá hòa.
- Đây là mô hình học tập, không mô phỏng đầy đủ mọi chi tiết của bộ lập lịch trong hệ điều hành thực.

## 4. Cơ sở lý thuyết
### 4.1. SJF không độc quyền
Khi CPU rảnh, chọn tiến trình đã đến có Burst Time nhỏ nhất. Khi đã bắt đầu, tiến trình chạy đến khi hoàn thành; tiến trình mới đến không làm ngắt tiến trình đang chạy.

### 4.2. SRTF có độc quyền
Tại mỗi thời điểm, chọn tiến trình đã đến có thời gian còn lại nhỏ nhất. Nếu một tiến trình mới đến có thời gian còn lại ngắn hơn, CPU có thể ngắt tiến trình hiện tại và chuyển sang tiến trình mới.

### 4.3. Công thức
- CT: thời điểm hoàn thành.
- TAT = CT - Arrival Time.
- WT = TAT - Burst Time.
- RT = thời điểm bắt đầu chạy lần đầu - Arrival Time.
- Thời gian chờ trung bình = tổng WT / số tiến trình.
- Turnaround trung bình = tổng TAT / số tiến trình.

## 5. Phân tích và thiết kế
### 5.1. Đầu vào
Danh sách tiến trình gồm PID, Arrival Time, Burst Time.

### 5.2. Đầu ra
Biểu đồ Gantt, bảng các chỉ số theo từng tiến trình và số liệu trung bình để so sánh hai thuật toán.

### 5.3. Quy trình xử lý
1. Nhập hoặc tải dữ liệu CSV.
2. Kiểm tra PID, Arrival Time, Burst Time.
3. Chạy SJF không độc quyền.
4. Chạy SRTF có độc quyền.
5. Tính các chỉ số.
6. Hiển thị biểu đồ Gantt và bảng kết quả.
7. Xuất kết quả CSV nếu cần.


## 7. Kiểm thử

Thực hiện ít nhất các trường hợp:
- Bộ dữ liệu thông thường có nhiều tiến trình đến ở các thời điểm khác nhau.
- Một tiến trình đến muộn, kiểm tra khoảng CPU rảnh.
- Chỉ có một tiến trình.
- Burst Time bằng nhau.
- Dữ liệu sai: PID trùng, Arrival Time âm, Burst Time bằng 0 hoặc nhập chữ.
- Kiểm tra đối chiếu biểu đồ Gantt với bảng kết quả và công thức.

| Trường hợp | Kết quả mong đợi |
|---|---|
| Dữ liệu hợp lệ | Chạy được cả hai thuật toán và hiện bảng kết quả |
| CPU không có tiến trình sẵn sàng | Hiện đoạn IDLE |
| PID trùng | Báo lỗi, không chạy mô phỏng |
| Burst Time <= 0 | Báo lỗi, không chạy mô phỏng |
| CSV sai cột | Báo định dạng CSV không hợp lệ |
```text
I.	Đối chiếu giải thuật 
A. SJF Không độc quyền
1.	Tiến trình điều phối theo thời gian (Trace):
•	t = 0: Chỉ có P1 (AT = 0, BT = 7) trong hàng đợi -> Cấp CPU cho P1. Do tính chất non-preemptive, P1 chạy liên tục từ 0 -> 7. Trong thời gian này, P2(2,4), P3(4,1), P4(5,3) lần lượt đến và xếp hàng chờ.
•	t = 7: P1 hoàn tất (CT = 7). Hàng đợi hiện có: {P2(BT = 4), P3(BT = 1), P4(BT = 3)}. Tiến trình có BT nhỏ nhất là P3(BT = 1) -> Chuyển CPU lần 1 sang P3. P3 chạy từ 7 -> 8.
•	t = 8: P3 hoàn tất (CT = 8). Đúng lúc này P5(AT = 8, BT = 2) vừa đến. Hàng đợi hiện có: {P2(BT = 4), P4(BT = 3), P5(BT = 2)}. Tiến trình có BT nhỏ nhất là P5(BT = 2) -> Chuyển CPU lần 2 sang P5. P5 chạy từ 8 -> 10.
•	t = 10: P5 hoàn tất (CT = 10). Hàng đợi còn: {P2(BT = 4), P4(BT = 3)}. Chọn P4(BT = 3)    -> Chuyển CPU lần 3 sang P4. P4 chạy từ 10 -> 13.
•	t = 13: P4 hoàn tất (CT = 13). Hàng đợi chỉ còn P2(BT = 4) -> Chuyển CPU lần 4 sang P2. P2 chạy từ 13 -> 17 và kết thúc.
2.	Biểu đồ Gantt.
|-------P1(7)-------|-P3(1)-|--P5(2)--|---P4(3)---|----P2(4)----|
0                   7       8         10          13            17
3.	Thống kê chuyển ngữ cảnh.
•	Số lần bị ngắt quãng: 0
•	Số lần chuyển CPU: 4
B. SRTF(Shortest Remaining Time First)
1. Tiến trình điều phối theo thời gian (Trace):
•	t = 0: Chỉ có P1 (AT = 0, BT = 7, Remaining = 7) trong hàng đợi -> Cấp CPU cho P1. P1 chạy từ 0 -> 2.
•	t = 2: P2 đến (AT = 2, BT = 4, Remaining = 4). Lúc này P1 còn lại 5 đơn vị thời gian (7 - 2 = 5). So sánh thấy P2 ngắn hơn P1 (4 < 5) -> Preemption (ngắt quãng lần 1): Tạm dừng P1, chuyển CPU sang P2. P2 chạy từ 2 -> 4.
•	t = 4: P3 đến (AT = 4, BT = 1, Remaining = 1). Lúc này P2 còn lại 2 đơn vị thời gian (4 - 2 = 2). So sánh thấy P3 ngắn hơn P2 (1 < 2) -> Preemption (ngắt quãng lần 2): Tạm dừng P2, chuyển CPU sang P3. P3 chạy từ 4 -> 5.
•	t = 5: P3 chạy xong và hoàn tất (CT = 5). Hàng đợi còn lại P1 (còn 5), P2 (còn 2) và P4 vừa đến lúc t = 5 (BT = 3). Tiến trình có thời gian còn lại ngắn nhất là P2 (còn 2) -> Cấp CPU cho P2 chạy tiếp từ 5 -> 7.
•	t = 7: P2 hoàn tất (CT = 7). Hàng đợi còn P1 (còn 5) và P4 (còn 3). Tiến trình có thời gian còn lại ngắn nhất là P4 (3 < 5) -> Chuyển CPU sang P4. P4 chạy từ 7 -> 10.
•	t = 10: P4 hoàn tất (CT = 10). Lúc này P5 đã đến từ t = 8 (BT = 2). Hàng đợi còn P1 (còn 5) và P5 (còn 2). Tiến trình có thời gian còn lại ngắn nhất là P5 (2 < 5) -> Chuyển CPU sang P5. P5 chạy từ 10 -> 12.
•	t = 12: P5 hoàn tất (CT = 12). Hàng đợi chỉ còn lại duy nhất P1 -> Cấp CPU cho P1 chạy nốt phần còn lại từ 12 -> 17 và kết thúc.
2.  Biểu đồ Gantt.
  (P1=5)    (P2=2)   
|--P1(2)--|--P2(2)--|-P3(1)-|--P2(2)--|---P4(3)---|--P5(2)--|-----P1(5)-----|
0         2         4       5         7          10         12               17
3. Thống kê chuyển ngữ cảnh.
Số lần bị ngắt quãng: 2
•	Số lần chuyển CPU: 6.
```
