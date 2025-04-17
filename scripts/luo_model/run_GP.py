"""
Simulate: The long-term proportion of type G individuals decreases in the
mutation rate v.
"""
import subprocess
import numpy as np
import pandas as pd
from tqdm import tqdm

# 参数
m = 50
n = 50
r = 0.2
s = 0.2
full_output = 0
uniform_init = 1

nu_values = np.linspace(0.1, 0.001, 0.000001,)  # 你要 sweep 的 mutation rate
num_repeats = 10

results = []

for nu in tqdm(nu_values):
    u50_values = []
    for repeat in range(num_repeats):
        output_path = f"./temp_output.csv"
        # 调用你的 main 可执行文件
        subprocess.run([
            "./main", output_path,
            str(m), str(n), str(r), str(s), str(nu),
            str(full_output), str(uniform_init)
        ])
        # 读取输出文件，提取最后一行的 u50
        with open(output_path, "r") as f:
            lines = f.readlines()
            last_line = lines[-1]
            parts = last_line.strip().split(",")
            u50 = float(parts[n])  # u50 在最后一列
            u50_values.append(u50)
    
    avg_u50 = np.mean(u50_values)
    results.append({"nu": nu, "avg_u50": avg_u50})

# 保存结果
df = pd.DataFrame(results)
df.to_csv("output.csv", index=False)
print(df)
