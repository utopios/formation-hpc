### Commande analyse

```bash
pbsnodes -a | grep -E "state|resources_available|resources_assigned"
qstat -f | grep -E "Job Id|queue|qtime|etime|start_time"

```


```python
import re
from datetime import datetime

log_file = "/var/spool/pbs/server_logs/20240628"
waiting_times = []

with open(log_file, "r") as file:
    start_times = {}
    for line in file:
        if "Job Start" in line:
            job_id_match = re.search(r'Job (\d+)', line)
            start_time_match = re.search(r'\((.*?)\)', line)
            if job_id_match and start_time_match:
                job_id = job_id_match.group(1)
                start_time = datetime.strptime(start_time_match.group(1), "%m/%d/%Y %H:%M:%S")
                start_times[job_id] = start_time
        elif "Job Queued" in line:
            job_id_match = re.search(r'Job (\d+)', line)
            queued_time_match = re.search(r'\((.*?)\)', line)
            if job_id_match and queued_time_match:
                job_id = job_id_match.group(1)
                queued_time = datetime.strptime(queued_time_match.group(1), "%m/%d/%Y %H:%M:%S")
                if job_id in start_times:
                    start_time = start_times[job_id]
                    waiting_time = (start_time - queued_time).total_seconds()
                    waiting_times.append(waiting_time)

if waiting_times:
    average_waiting_time = sum(waiting_times) / len(waiting_times)
    print(f"Average job waiting time: {average_waiting_time} seconds")
else:
    print("No waiting times found.")
```