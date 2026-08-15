# 🚀 XAE Language Engine

Aplicação web para codificar/decodificar texto em XAE (linguagem fictícia).

## ⚡ Início Rápido

### Windows
1. **Instale Python** (se ainda não tem):
   - Acesse: https://python.org/downloads
   - Baixe Python 3.9 ou superior
   - **MARQUE: "Add Python to PATH"** ✓
   - Execute o instalador

2. **Execute o script de startup**:
   - Duplo-clique em `rodar.bat`
   - Aguarde as dependências serem instaladas
   - Abra seu navegador em: http://127.0.0.1:5000

### macOS/Linux
```bash
cd /caminho/para/XAE
python3 -m pip install -r requirements.txt
python3 app.py
```

Depois acesse: http://127.0.0.1:5000

## 📋 Requisitos

- Python 3.9+
- Flask 3.0+
- C++ compilador (para compilar `xae_codec.cpp`)

## 🔧 Estrutura

```
XAE/
├── app.py                 # Servidor Flask
├── requirements.txt       # Dependências Python
├── rodar.bat             # Script de startup (Windows)
├── cpp/
│   ├── xae_codec.cpp     # Motor de codificação (C++)
│   ├── xae_codec.c       # Versão em C puro
│   └── xae_codec.exe     # Executável compilado
└── templates/
    └── index.html        # Interface web
    static/
    └── style.css         # Estilos
```

## 🐛 Troubleshooting

**Python não encontrado?**
- Reinstale Python e marque "Add Python to PATH"
- Reinicie o computador

**Motor C++ não encontrado?**
- Compile: `g++ cpp/xae_codec.cpp -o cpp/xae_codec.exe`

**Porta 5000 já em uso?**
- Edite `app.py` e mude a porta em `app.run()`

---

**Desenvolvido para XAE Language Project**
