@echo off
chcp 65001 >nul
cls

echo.
echo ========================================
echo  XAE - INICIALIZAR APLICAÇÃO
echo ========================================
echo.

:: Verificar se Python está instalado
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ ERRO: Python não está instalado!
    echo.
    echo Baixe Python em: https://python.org/downloads
    echo.
    echo ⚠️  IMPORTANTE durante a instalação:
    echo   - Marque "Add Python to PATH"
    echo.
    pause
    exit /b 1
)

echo ✓ Python encontrado
python --version
echo.

:: Instalar dependências
echo Instalando dependências...
python -m pip install --upgrade pip >nul 2>&1
python -m pip install -r requirements.txt

if %errorlevel% neq 0 (
    echo ❌ Erro ao instalar dependências
    pause
    exit /b 1
)

echo ✓ Dependências instaladas
echo.

:: Verificar se existe o executável C++
if not exist "cpp\xae_codec.exe" (
    echo ⚠️  AVISO: cpp/xae_codec.exe não encontrado
    echo Certifique-se de compilar cpp/xae_codec.cpp
    echo.
)

:: Iniciar servidor
echo ========================================
echo  Iniciando servidor...
echo ========================================
echo.
echo 🌐 Acesse: http://127.0.0.1:5000
echo.
echo (Pressione Ctrl+C para parar)
echo ========================================
echo.

python app.py
pause
