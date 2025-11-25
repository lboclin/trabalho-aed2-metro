import pandas as pd
import matplotlib.pyplot as plt
import imageio.v2 as imageio
import os
import glob

# --- Configurações ---
input_folder = 'arquivos_relacionados/saidas_cpp'
output_folder = 'arquivos_relacionados/plots'
gif_name = 'timelapse_smart_x.gif'

if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# Busca arquivos com o padrão "smart"
files = glob.glob(os.path.join(input_folder, "grafo_visual_smart_*.csv"))

if not files:
    print("Nenhum CSV 'smart' encontrado. Rode 'make run' primeiro.")
    exit()

# Função para extrair o fator X (float) do nome do arquivo
def get_factor(filename):
    try:
        base = os.path.basename(filename)
        # Remove prefixo e extensão para sobrar só o número (ex: "1.2")
        val_str = base.replace('grafo_visual_smart_', '').replace('.csv', '')
        return float(val_str)
    except:
        return -1.0

# Ordena os arquivos pelo Fator em ordem DECRESCENTE (do maior para o menor)
# Maior fator (3.0) = Menos túneis (Início)
# Menor fator (1.2) = Mais túneis (Fim)
files.sort(key=get_factor, reverse=True)

frames = []
temp_frames_dir = os.path.join(output_folder, 'temp_frames_x')
if not os.path.exists(temp_frames_dir):
    os.makedirs(temp_frames_dir)

print(f"Gerando quadros para {len(files)} cenários Smart...")

# --- Loop de Geração de Quadros ---
for i, file_path in enumerate(files):
    factor_val = get_factor(file_path)
    print(f" -> Desenhando quadro Fator={factor_val}...")

    try:
        df = pd.read_csv(file_path)
    except Exception as e:
        print(f"Erro ao ler {file_path}: {e}")
        continue
    
    # Configura o Plot (Tamanho HD)
    plt.figure(figsize=(10, 10), dpi=100)
    plt.title(f"Algoritmo Smart: Fator de Tolerância {factor_val}x", fontsize=15)
    plt.xlabel("Longitude")
    plt.ylabel("Latitude")
    
    # Trava os eixos nas coordenadas de NY
    plt.xlim(-74.25, -73.70) 
    plt.ylim(40.50, 40.92)
    plt.grid(True, linestyle=':', alpha=0.3)
    
    # Separa dados
    df_mst = df[df['type'] == 'MST']
    df_new = df[df['type'] == 'New_Connection']
    
    # 1. Desenha MST (Azul) - Espinha dorsal fixa
    for _, row in df_mst.iterrows():
        plt.plot([row['lon1'], row['lon2']], [row['lat1'], row['lat2']], 
                 color='#1f78b4', linewidth=1.0, alpha=0.7, zorder=1)

    # 2. Desenha Novas Conexões (Vermelho) - Destaque
    for _, row in df_new.iterrows():
        plt.plot([row['lon1'], row['lon2']], [row['lat1'], row['lat2']], 
                 color='red', linewidth=2.0, alpha=1.0, zorder=2)

    # 3. Desenha os Nós (Estações)
    all_lons = pd.concat([df['lon1'], df['lon2']])
    all_lats = pd.concat([df['lat1'], df['lat2']])
    plt.scatter(all_lons, all_lats, s=5, color='black', zorder=3)

    # Salva o quadro temporário
    # Usamos o índice 'i' no nome para garantir a ordem correta no sistema de arquivos
    frame_path = os.path.join(temp_frames_dir, f"frame_{i:03d}.png")
    plt.savefig(frame_path)
    plt.close() 
    
    frames.append(imageio.imread(frame_path))

# --- Geração do GIF ---
print("Compilando GIF Smart...")
gif_path = os.path.join(output_folder, gif_name)

# duration=1000ms = 1 segundo por fator
imageio.mimsave(gif_path, frames, duration=1000, loop=0)

print(f"✅ Timelapse Smart salvo em: {gif_path}")

# Limpeza opcional
# import shutil
# shutil.rmtree(temp_frames_dir)