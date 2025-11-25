import pandas as pd
import matplotlib.pyplot as plt
import os

csv_path = 'arquivos_relacionados/resumo_metricas_x.csv'
output_folder = 'arquivos_relacionados/plots'

# Garante que a pasta existe
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

try:
    df = pd.read_csv(csv_path)
    df = df[df['Fator'] != 100]
    df = df.sort_values(by='Fator', ascending=False)
except:
    print("Arquivo não encontrado.")
    exit()

fig, ax1 = plt.subplots(figsize=(10, 6))

# Título e Labels
plt.title('Trade-off: Custo de Construção vs. Eficiência de Transporte', fontsize=14)
ax1.set_xlabel('Fator de Alongamento Máximo (x vezes a reta)', fontsize=12)

# --- EIXO 1: Custo (Vermelho) ---
ax1.set_ylabel('Custo Total (Bilhões $)', color='tab:red', fontsize=12)
ax1.plot(df['Fator'], df['Custo_Bilhoes'], color='tab:red', marker='o', label='Custo ($)')
ax1.tick_params(axis='y', labelcolor='tab:red')
ax1.grid(True, linestyle='--', alpha=0.5)

# --- EIXO 2: Eficiência (Azul) ---
ax2 = ax1.twinx()
ax2.set_ylabel('Eficiência (Km/Viagem)', color='tab:blue', fontsize=12)
ax2.plot(df['Fator'], df['Eficiencia_Km_Viagem'], color='tab:blue', marker='s', linestyle='--', label='Eficiência (km)')
ax2.tick_params(axis='y', labelcolor='tab:blue')

# Anotações nos pontos
for i, txt in enumerate(df['Fator']):
    ax1.annotate(f"${df['Custo_Bilhoes'][i]:.1f}B", (df['Fator'][i], df['Custo_Bilhoes'][i]), textcoords="offset points", xytext=(0,10), ha='center', color='red')

# Isso inverte visualmente o eixo X.
# Esquerda: Fatores Altos (Barato/Ineficiente) -> Direita: Fatores Baixos (Caro/Eficiente)
ax1.invert_xaxis() 

plt.tight_layout()
output_path = os.path.join(output_folder, 'grafico_metrics_x.png')
plt.savefig(output_path, dpi=300)
print(f"Gráfico Smart invertido gerado em: {output_path}")