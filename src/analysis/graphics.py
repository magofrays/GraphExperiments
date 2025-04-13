import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.colors as mcolors  
import random
import json

colors = list(mcolors.get_named_colors_mapping().keys())

def draw_random_karger_test():
    test_random_karger = dict(json.load(open("test_random_karger.json")))
    prob_start = test_random_karger["prob_start"]
    prob_step = test_random_karger["prob_step"]
    karger_probabilities = test_random_karger["karger_probabilities"]
    graph_size = test_random_karger["graph_size"]
    karger_iters = test_random_karger["karger_iters"]
    pdf = PdfPages("test_random_karger.pdf")
    x = [prob_start + prob_step*i for i in range(len(karger_probabilities))]
    figure = plt.figure()
    axes = figure.subplots()
    axes.plot(x, karger_probabilities, label=f"real probability")
    teor_prob = [1 - (1 - 2 / (graph_size * (graph_size - 1)))**karger_iters for i in range(len(karger_probabilities))]
    axes.plot(x, teor_prob, label=f"teor probability")
    axes.set_xlabel(f"graph probabilities, size:{graph_size}")
    axes.set_ylabel(f"karger probabilities, iters:{karger_iters}")
    axes.set_title("График вероятности каргера от вероятностного связного графа")
    axes.legend()
    pdf.savefig(figure)
    plt.close()
    pdf.close()

def draw_full_karger_test():
    test_full_karger = dict(json.load(open("test_full_karger.json")))
    sizes = test_full_karger["sizes"]
    figure = plt.figure()
    pdf = PdfPages("test_full_karger.pdf")
    axes = figure.subplots()
    axes.set_xlabel(f"karger iterations")
    axes.set_ylabel(f"karger probability")
    axes.set_title("График вероятности алгоритма от количества итераций")
    for s in sizes:
        random_color = random.choice(colors)
        probabilities = test_full_karger[str(s)]
        x = [1+i*5 for i in range(0, len(probabilities))]
        teor_prob = [1 - (1 - 2/(s*(s-1)))**k for k in x]
        axes.plot(x, probabilities, label=f"real_prob({s})", color=random_color)
        axes.plot(x, teor_prob, label=f"teor_prob({s})", color=random_color)        
    axes.legend()
    pdf.savefig(figure)
    plt.close()
    pdf.close()
    
def draw_full_karger_test_reversed():
    test_full_karger = dict(json.load(open("test_full_karger.json")))
    sizes = test_full_karger["sizes"]
    figure = plt.figure()
    pdf = PdfPages("test_full_karger_reversed.pdf")
    axes = figure.subplots()
    axes.set_xlabel(f"karger iterations")
    axes.set_ylabel(f"karger probability")
    axes.set_title("График вероятности алгоритма от количества итераций")
    for s in sizes:
        random_color = random.choice(colors)
        probabilities = [1 - y for y in test_full_karger[str(s)]]
        x = [1+i*5 for i in range(0, len(probabilities))]
        teor_prob = [(1 - 2/(s*(s-1)))**k for k in x]
        axes.plot(x, probabilities, label=f"real_prob({s})", color=random_color)
        axes.plot(x, teor_prob, label=f"teor_prob({s})", color=random_color)        
    axes.legend()
    pdf.savefig(figure)
    plt.close()
    pdf.close()

def draw_cyclomatic_test():
    test_cyclomatic = dict(json.load(open("test_cyclomatic.json")))
    sizes = test_cyclomatic["sizes"]
    prob_start = test_cyclomatic["prob_start"]
    prob_step = test_cyclomatic["prob_step"]
    figure = plt.figure()
    pdf = PdfPages("test_cyclomatic.pdf")
    axes = figure.subplots()
    axes.set_xlabel(f"graph probability")
    axes.set_ylabel(f"cyclomatic number")
    axes.set_title("График зависимости цикломатического числа от вероятности графа")
    for s in sizes:
        probabilities = test_cyclomatic[str(s)]
        x = [prob_start + i*prob_step for i in range(len(probabilities))]
        axes.plot(x, probabilities, label=f"graph({s})")
    axes.legend()
    pdf.savefig(figure)
    plt.close()
    pdf.close()

def draw_component_test():
    test_components = dict(json.load(open("test_components.json")))
    sizes = test_components["sizes"]
    prob_start = test_components["prob_start"]
    prob_step = test_components["prob_step"]
    figure = plt.figure()
    pdf = PdfPages("test_components.pdf")
    axes = figure.subplots()
    axes.set_xlabel(f"graph probability")
    axes.set_ylabel(f"connected components")
    axes.set_title("Зависимость количества компонент связности от вероятности графа")
    for s in sizes:
        probabilities = test_components[str(s)]
        x = [prob_start + i*prob_step for i in range(len(probabilities))]
        axes.plot(x, probabilities, label=f"graph({s})")
    axes.legend()
    pdf.savefig(figure)
    plt.close()
    pdf.close()

def draw_diameter_test():
    test_diameter = dict(json.load(open("test_diameter.json")))
    sizes = test_diameter["sizes"]
    prob_start = test_diameter["prob_start"]
    prob_step = test_diameter["prob_step"]
    figure = plt.figure()
    pdf = PdfPages("test_diameter.pdf")
    axes = figure.subplots()
    axes.set_xlabel(f"graph probability")
    axes.set_ylabel(f"diameter size")
    axes.set_title("Зависимость длинны диаметра от вероятности связного графа")
    for s in sizes:
        probabilities = test_diameter[str(s)]
        x = [prob_start + i*prob_step for i in range(len(probabilities))]
        axes.plot(x, probabilities, label=f"graph({s})")
    # axes.legend()
    pdf.savefig(figure)
    plt.close()
    pdf.close()    

draw_diameter_test()
        
    