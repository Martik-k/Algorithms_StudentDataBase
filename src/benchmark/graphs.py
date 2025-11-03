import pandas as pd
import matplotlib.pyplot as plt

df_ops = pd.read_csv('matrix.csv')
df_ops.set_index('SampleSize', inplace=True)

plt.figure(figsize=(8, 5))
for column in df_ops.columns:
    plt.plot(df_ops.index, df_ops[column], marker='o', label=column)

plt.xlabel('Sample Size')
plt.ylabel('Number of Operations (in 10s)')
plt.title('Performance Comparison: DB1 vs DB2 vs DB3')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.tight_layout()
plt.savefig('../../results/result_ops.png', dpi=300)
plt.show()


df_mem = pd.read_csv('memory.csv')
df_mem.set_index('SampleSize', inplace=True)

plt.figure(figsize=(8, 5))
for column in df_mem.columns:
    plt.plot(df_mem.index, df_mem[column], marker='s', label=column)

plt.xlabel('Sample Size')
plt.ylabel('Peak Memory Usage (kB)')
plt.title('Memory Usage: DB1 vs DB2 vs DB3')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.tight_layout()
plt.savefig('../../results/result_mem.png', dpi=300)
plt.show()