import pandas as pd
import matplotlib.pyplot as plt
import os

csv_path = 'arquivos_relacionados/resumo_metricas_x.csv'
output_folder = 'arquivos_relacionados/plots'

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

try:
    df = pd.read_csv(csv_path)
    df = df.sort_values(by='Custo_Bilhoes')
    
except FileNotFoundError:
    print("Erro: Arquivo não encontrado.")
    exit()

plt.figure(figsize=(10, 6))

plt.plot(df['Custo_Bilhoes'], df['Eficiencia_Km_Viagem'], 
         marker='o', linestyle='-', color='#2c3e50', linewidth=2, label='Trade-off')

for index, row in df.iterrows():
    x_val = int(row['Fator'])
    cost = row['Custo_Bilhoes']
    eff = row['Eficiencia_Km_Viagem']
    
    plt.annotate(f'x={x_val}', (cost, eff), textcoords="offset points", xytext=(10, 5), ha='left', fontsize=11, fontweight='bold', color='#c0392b')

plt.title('Curva de Eficiência (Com Algoritmo Smart)', fontsize=14)
plt.xlabel('Custo Total de Construção (Bilhões $)', fontsize=12)
plt.ylabel('Distância Média de Viagem (km)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.6)

output_file = os.path.join(output_folder, 'grafico_pareto_x.png')
plt.savefig(output_file, dpi=300)
print(f"Gráfico Pareto salvo em: {output_file}")