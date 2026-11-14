# OS_SCH_02 - SJF và SRTF

## Mô tả
Ứng dụng desktop C++ mô phỏng:
- SJF không độc quyền (Non-preemptive)
- SRTF có độc quyền (Preemptive)
- Biểu đồ Gantt
- Completion Time, Turnaround Time, Waiting Time
- Đọc/lưu input CSV và xuất kết quả CSV

## Yêu cầu
- C++

## Cách chạy trên Windows
1. Giải nén thư mục dự án.
2. Mở thư mục bằng VS Code.
3. Mở Terminal tại thư mục dự án.
4. Dữ liệu mẫu được nạp sẵn. Nhấn `Chạy mô phỏng`.
5. Có thể thêm/xóa tiến trình hoặc tải `input.csv`.

## Định dạng CSV đầu vào
CSV phải có ba cột: `pid,arrival,burst`.
- `Process_ID`: mã tiến trình, không trùng.
- `Arrival_Time`: thời điểm đến, số nguyên >= 0.
- `Burst_Time`: thời gian CPU, số nguyên > 0.

## Công thức
- Completion Time (CT): thời điểm tiến trình hoàn thành.
- Turnaround Time (TAT) = CT - Arrival Time.
- Waiting Time (WT) = TAT - Burst Time.

## Cấu trúc
- `main.cpp`: mã nguồn chương trình.
- `input.csv`: dữ liệu mẫu.
- `report/report.md`: khung báo cáo để hoàn thiện.
- `output/output_sjf_np.csv`: nơi lưu kết quả xuất ra.
- `output/output_srtf.csv`: nơi lưu kết quả xuất ra.

## Lưu ý
Đây là mô phỏng lập lịch CPU ở mức thuật toán. Thời gian chuyển ngữ cảnh thực tế được giả định bằng 0; số lần chuyển tiến trình được minh họa trong biểu đồ.
