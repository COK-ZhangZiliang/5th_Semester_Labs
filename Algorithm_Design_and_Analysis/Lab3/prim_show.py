import networkx as nx
import matplotlib.pyplot as plt

nodes = [22, 42]

for node in nodes:
    # 读取数据
    G = nx.Graph()
    with open(f'DATA/mst_{node}.txt', 'r') as f:
        lines = f.readlines()
        for i in range(0, len(lines)):
            index1, index2, _, _, weight = map(float, lines[i].split())
            G.add_node(int(index1))
            G.add_node(int(index2))
            if weight != -1.0:
                G.add_edge(index1, index2, weight=weight)

    # 读取结果
    red_edges = []
    with open(f'RESULT/prim_{node}_res.txt', 'r') as f:
        lines = f.readlines()
        for i in range(0, len(lines)):
            red_edges.append((i + 2, int(lines[i].strip())))

    # 绘制图形
    plt.figure(figsize=(8, 6))
    pos = nx.kamada_kawai_layout(G)
    nx.draw(G, pos, node_color='lightblue', edge_color='lightblue', with_labels=True, font_size=10, node_size=100)
    nx.draw_networkx_edges(G, pos, edgelist=red_edges, edge_color='red', width=2)
    plt.axis('off')
    plt.savefig(f'RESULT/prim_{node}.png')
