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
