from flask import Flask, render_template, request, jsonify
from pathlib import Path
import subprocess
import platform

app = Flask(__name__)

BASE_DIR = Path(__file__).resolve().parent

if platform.system() == "Windows":
    CPP_BIN = BASE_DIR / "cpp" / "xae_codec.exe"
else:
    CPP_BIN = BASE_DIR / "cpp" / "xae_codec"


def run_cpp(mode, text):
    """
    Executa o motor C++.

    mode:
        encode -> Português para XAE
        decode -> XAE para Português
    """

    if not CPP_BIN.exists():
        return {
            "ok": False,
            "error": (
                "Motor C++ não encontrado. "
                "Compile cpp/xae_codec.cpp primeiro."
            )
        }

    try:
        result = subprocess.run(
            [str(CPP_BIN), mode, text],
            capture_output=True,
            text=True,
            encoding="utf-8",
            timeout=5
        )

        if result.returncode != 0:
            return {
                "ok": False,
                "error": result.stderr.strip()
            }

        return {
            "ok": True,
            "result": result.stdout.strip()
        }

    except subprocess.TimeoutExpired:
        return {
            "ok": False,
            "error": "O processamento demorou demais."
        }

    except Exception as e:
        return {
            "ok": False,
            "error": str(e)
        }


@app.route("/")
def home():
    return render_template("index.html")


@app.post("/api/encode")
def encode():

    data = request.get_json(silent=True) or {}

    text = str(data.get("text", ""))

    if not text.strip():
        return jsonify({
            "ok": False,
            "error": "Digite algum texto."
        })

    return jsonify(
        run_cpp("encode", text)
    )


@app.post("/api/decode")
def decode():

    data = request.get_json(silent=True) or {}

    text = str(data.get("text", ""))

    if not text.strip():
        return jsonify({
            "ok": False,
            "error": "Digite um código XAE."
        })

    return jsonify(
        run_cpp("decode", text)
    )


if __name__ == "__main__":

    print()
    print("=" * 60)
    print("              XAE LANGUAGE ENGINE")
    print("=" * 60)
    print("Servidor: http://127.0.0.1:5000")
    print("=" * 60)
    print()

    app.run(
        host="127.0.0.1",
        port=5000,
        debug=True
    )
