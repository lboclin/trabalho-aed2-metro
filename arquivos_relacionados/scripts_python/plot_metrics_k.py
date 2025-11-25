import pandas as pd
import matplotlib.pyplot as plt

# 1. Carregar os dados gerados pelo C++
# O caminho é relativo de onde você roda o script. 
# Vamos assumir que você roda da raiz do projeto.
csv_path = 'arquivos_relacionados/resumo_metricas_k.csv'

try:
    df = pd.read_csv(csv_path)
except FileNotFoundError:
    print(f"Erro: O arquivo {csv_path} não foi encontrado.")
    print("Rode 'make run' primeiro para gerar os dados!")
    exit()

# 2. Configurar o Gráfico (Dual Axis)
fig, ax1 = plt.subplots(figsize=(10, 6))

# Título e Eixo X (K)
plt.title('Trade-off: Custo de Construção vs. Eficiência de Transporte', fontsize=14)
ax1.set_xlabel('K (Vizinhos Extras Adicionados)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.6)

# --- Eixo Y da Esquerda (Custo - Vermelho) ---
color_cost = 'tab:red'
ax1.set_ylabel('Custo Total (Bilhões $)', color=color_cost, fontsize=12)
line1 = ax1.plot(df['K'], df['Custo_Bilhoes'], color=color_cost, marker='o', label='Custo ($)')
ax1.tick_params(axis='y', labelcolor=color_cost)

# --- Eixo Y da Direita (Eficiência - Azul) ---
ax2 = ax1.twinx()  # Cria o segundo eixo compartilhando o mesmo X
color_eff = 'tab:blue'
ax2.set_ylabel('Distância Média de Viagem (km)', color=color_eff, fontsize=12)
# Note que queremos que a eficiência "baixa" (distância alta) fique clara, 
# então a curva vai descer conforme K aumenta.
line2 = ax2.plot(df['K'], df['Eficiencia_Km_Viagem'], color=color_eff, marker='s', linestyle='--', label='Distância Média (km)')
ax2.tick_params(axis='y', labelcolor=color_eff)

# Combinar as legendas dos dois eixos
lines = line1 + line2
labels = [l.get_label() for l in lines]
ax1.legend(lines, labels, loc='center right')

# Anotações nos pontos
for i, txt in enumerate(df['K']):
    ax1.annotate(f"${df['Custo_Bilhoes'][i]:.1f}B", (df['K'][i], df['Custo_Bilhoes'][i]), textcoords="offset points", xytext=(0,10), ha='center', color='red')

# 3. Salvar e Mostrar
output_path = 'arquivos_relacionados/plots/grafico_metrics_k.png'
plt.tight_layout()
plt.savefig(output_path, dpi=300)
print(f"Gráfico salvo com sucesso em: {output_path}")

# Se estiver rodando localmente e quiser ver a janela:
plt.show()