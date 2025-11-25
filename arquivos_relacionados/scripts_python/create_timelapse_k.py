import pandas as pd
import matplotlib.pyplot as plt
import imageio.v2 as imageio
import os
import glob

# --- Configurações ---
input_folder = 'arquivos_relacionados/saidas_cpp'
output_folder = 'arquivos_relacionados/plots'
gif_name = 'timelapse_evolucao.gif'

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# Busca arquivos
files = glob.glob(os.path.join(input_folder, "grafo_visual_k*.csv"))

if not files:
    print("Nenhum CSV encontrado. Rode 'make run' primeiro.")
    exit()

# Função para extrair o número K e ordenar corretamente (0, 1, 2, 10...)
def get_k(filename):
    try:
        base = os.path.basename(filename)
        return int(base.replace('grafo_visual_k', '').replace('.csv', ''))
    except:
        return 9999

# Ordena os arquivos numericamente pelo K
files.sort(key=get_k)

frames = []
temp_frames_dir = os.path.join(output_folder, 'temp_frames_k')
if not os.path.exists(temp_frames_dir):
    os.makedirs(temp_frames_dir)

print(f"Gerando quadros para {len(files)} cenários...")

# --- Loop de Geração de Quadros ---
for file_path in files:
    k_val = get_k(file_path)
    print(f" -> Desenhando quadro K={k_val}...")

    df = pd.read_csv(file_path)
    
    # Configura o Plot (Tamanho HD)
    plt.figure(figsize=(10, 10), dpi=100)
    plt.title(f"Evolução da Rede de Metrô: Cenário K={k_val}", fontsize=15)
    plt.xlabel("Longitude")
    plt.ylabel("Latitude")
    
    # Trava os eixos para o mapa não ficar "sambando"
    # Coordenadas aproximadas de NY para manter o zoom fixo
    plt.xlim(-74.25, -73.70) 
    plt.ylim(40.50, 40.92)
    plt.grid(True, linestyle=':', alpha=0.3)
    
    # Fundo escuro para ficar bonito (Opcional)
    # plt.style.use('dark_background') 

    # Separa dados
    df_mst = df[df['type'] == 'MST']
    df_new = df[df['type'] == 'New_Connection']
    
    # 1. Desenha MST (Azul)
    # Dica de performance: Plotar todas as linhas de uma vez usando coleção seria melhor, 
    # mas loop simples funciona bem para N=500.
    for _, row in df_mst.iterrows():
        plt.plot([row['lon1'], row['lon2']], [row['lat1'], row['lat2']], 
                 color='#1f78b4', linewidth=0.8, alpha=0.6, zorder=1)

    # 2. Desenha Novas Conexões (Vermelho Vibrante)
    for _, row in df_new.iterrows():
        plt.plot([row['lon1'], row['lon2']], [row['lat1'], row['lat2']], 
                 color='red', linewidth=2.0, alpha=1.0, zorder=2)

    # 3. Desenha os Nós (Estações) como pontinhos
    # Pegamos todas as coordenadas únicas de origem e destino
    all_lons = pd.concat([df['lon1'], df['lon2']])
    all_lats = pd.concat([df['lat1'], df['lat2']])
    plt.scatter(all_lons, all_lats, s=5, color='black', zorder=3, label='Estações')

    # Salva o quadro temporário
    frame_path = os.path.join(temp_frames_dir, f"frame_k{k_val}.png")
    plt.savefig(frame_path)
    plt.close() # Libera memória
    
    # Adiciona à lista de leitura do GIF
    frames.append(imageio.imread(frame_path))

# --- Geração do GIF ---
print("Compilando GIF...")
gif_path = os.path.join(output_folder, gif_name)
# duration é o tempo por quadro em milissegundos (ou segundos dependendo da versão)
# loop=0 faz repetir para sempre
imageio.mimsave(gif_path, frames, duration=1000, loop=0)

print(f"✅ Timelapse salvo em: {gif_path}")

# Limpeza (Opcional: remove os pngs temporários)
# import shutil
# shutil.rmtree(temp_frames_dir)