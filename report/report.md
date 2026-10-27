```text
                                           SJF Non-Preemptive (Shortest Job First)
ALGORITHM SJF_Non_Preemptive(Process_List)
Input: Danh sách tiến trình với Arrival_Time và Burst_Time
Output: Mô phỏng chạy không ngắt (Non-preemptive) và thời gian hoàn thành

1.  current_time = 0
2.  completed_count = 0
3.  total_processes = length(Process_List)
4.  running_process = NULL
5.  last_pid = NULL
6.  context_switch = 0
7.  Khởi tạo ready_queue rỗng
8.  Khởi tạo completed_list rỗng
9.  Khởi tạo gantt rỗng

10. WHILE completed_count < total_processes DO
        // 1. Cập nhật tiến trình đã đến
        FOR EACH process P IN Process_List DO
            IF P.Arrival_Time <= current_time AND P NOT IN ready_queue AND P NOT IN completed_list THEN
                Add P to ready_queue
            END IF
        END FOR

        // 2. Chọn tiến trình (KHÔNG có preemption, chỉ chọn khi CPU rảnh)
        IF running_process == NULL AND ready_queue IS NOT EMPTY THEN
            // Hòa Burst -> chọn Arrival_Time nhỏ hơn -> hòa nữa -> chọn ID nhỏ hơn
            P_shortest = Process in ready_queue with MIN(Burst_Time)
            Remove P_shortest from ready_queue
            running_process = P_shortest
            PRINT "CPU bắt đầu chạy tiến trình: " + running_process.id + " tại t = " + current_time

            IF last_pid != NULL AND last_pid != running_process.id THEN
                context_switch = context_switch + 1
            END IF
            last_pid = running_process.id
        END IF

        // Bổ sung: Xử lý trường hợp CPU rảnh rỗi (IDLE)
        IF running_process == NULL AND ready_queue IS EMPTY THEN
            PRINT "CPU IDLE tại t = " + current_time
            last_pid = NULL
            current_time = current_time + 1
            CONTINUE
        END IF

        // 3. Thực thi một mạch đến hết Burst_Time (không bị ngắt)
        start_time = current_time
        current_time = current_time + running_process.Burst_Time
        Add (running_process.id, start_time, current_time) to gantt

        running_process.Completion_Time = current_time
        running_process.Turnaround_Time = running_process.Completion_Time - running_process.Arrival_Time
        running_process.Waiting_Time = running_process.Turnaround_Time - running_process.Burst_Time
        Add running_process to completed_list
        PRINT "Tiến trình " + running_process.id + " hoàn thành tại t = " + current_time

        running_process = NULL
        completed_count = completed_count + 1
    END WHILE

    avg_waiting = SUM(P.Waiting_Time) / total_processes
    avg_turnaround = SUM(P.Turnaround_Time) / total_processes
    PRINT "Gantt chart: " + gantt
    PRINT "Số lần chuyển CPU (Context Switch): " + context_switch
    PRINT "Số lần Preemption: 0"
    PRINT "Avg Waiting: " + avg_waiting + " | Avg Turnaround: " + avg_turnaround
END Algorithm
```
```text
                                                   SRTF (Shortest Remaining Time First)
ALGORITHM SRTF(Process_List) 
Input: Danh sách tiến trình với Arrival_Time và Burst_Time 
Output: Mô phỏng cơ chế Preemption và thời gian hoàn thành 
  
1.  current_time = 0 
2.  completed_count = 0 
3.  total_processes = length(Process_List) 
4.  running_process = NULL 
5.  Khởi tạo ready_queue rỗng 
6.  Khởi tạo completed_list rỗng 
  
7.  WHILE completed_count < total_processes DO 
        // 1. Cập nhật tiến trình mới đến 
        FOR EACH process P IN Process_List DO 
            IF P.Arrival_Time == current_time THEN 
                P.Remaining_Time = P.Burst_Time 
                Add P to ready_queue 
            END IF 
        END FOR 
  
        // 2. Kiểm tra cơ chế PREEMPTION
        IF ready_queue IS NOT EMPTY THEN 
            P_shortest = Process in ready_queue with MIN(Remaining_Time) 
  
            IF running_process == NULL THEN 
                Remove P_shortest from ready_queue 
                running_process = P_shortest 
                PRINT "CPU bắt đầu chạy tiến trình: " + running_process.id + " tại t = " + current_time 
             
            ELSE IF P_shortest.Remaining_Time < running_process.Remaining_Time THEN 
                // --- XẢY RA PREEMPTION --- 
                PRINT ">>> PREEMPTION tại t = " + current_time + ": Tiến trình " + running_process.id + " bị ngắt bởi " + P_shortest.id 
                 
                Add running_process back to ready_queue 
                Remove P_shortest from ready_queue 
                running_process = P_shortest 
                 
                PRINT "CPU Switch: Chuyển sang chạy tiến trình " + running_process.id 
            END IF 
        END IF 

        // Bổ sung: Xử lý trường hợp CPU rảnh rỗi (IDLE) 
        IF running_process == NULL AND ready_queue IS EMPTY THEN 
            PRINT "CPU IDLE tại t = " + current_time 
            current_time = current_time + 1 
            CONTINUE 
        END IF 
  
        // 3. Thực thi tiến trình đang chạy trong 1 đơn vị thời gian 
        IF running_process IS NOT NULL THEN 
            running_process.Remaining_Time = running_process.Remaining_Time - 1 
  
            IF running_process.Remaining_Time == 0 THEN 
                current_time_completed = current_time + 1 
                running_process.Completion_Time = current_time_completed 
                Add running_process to completed_list 
                PRINT "Tiến trình " + running_process.id + " hoàn thành tại t = " + current_time_completed 
                 
                running_process = NULL 
                completed_count = completed_count + 1 
            END IF 
        END IF 
  
        // 4. Tăng thời gian hệ thống 
        current_time = current_time + 1 
    END WHILE 
END Algorithm
