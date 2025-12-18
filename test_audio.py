# Authors: Jose Martinez-Ponce, Malik Tragna
# Date: 12/15/2025
# Purpose: To create two seperate audio signals from LEFT and RIGHT
#          via AUX cable.

import numpy as np
import sounddevice as sd
import soundfile as sf

# ===================
# Parameters
# ===================
# decided to hard codes these in bc its easier

AUDIO_PATH   = r"C:\CPE643\final_proj\fir_lms\desired.wav"
NOISE_TYPE   = "white"
TONE_HZ      = 2000
NOISE_GAIN   = 0.80
REF_GAIN     = 0.80
SEED         = 7  

audio, fs = sf.read(AUDIO_PATH, dtype="float32")

# Mono (Take left channel)
if audio.ndim == 2:
    audio = audio[:, 0]

# Normalize desired audio
peak = np.max(np.abs(audio)) + 1e-9
audio = 0.8 * (audio / peak)

N = len(audio)
t = np.arange(N, dtype=np.float32) / fs  

# ===================
# Generate Noise
# ===================

rng = np.random.default_rng(SEED)

if NOISE_TYPE == "white":
    noise = rng.standard_normal(N).astype(np.float32)
    noise = noise / (np.max(np.abs(noise)) + 1e-9)
elif NOISE_TYPE == "tone":
    noise = np.sin(2 * np.pi * TONE_HZ * t).astype(np.float32)
else:
    raise ValueError("NOISE_TYPE must be either 'white' or 'tone'")

# ====================
# Build Stereo Signals
# ====================

primary = audio + NOISE_GAIN * noise   # left: desired + noise
reference = REF_GAIN * noise           # right: noise only

primary = np.clip(primary, -1.0, 1.0)  # fixed primarym + commas

stereo = np.column_stack([primary, reference]).astype(np.float32)

print(f"Playing {AUDIO_PATH} at {fs} Hz")
print("Left = desired + noise (primary)")
print("Right = noise only (ref)")

sd.play(stereo, fs)
sd.wait()

print("Done.")
