# rc4ok

**RC4OK** is a high‑performance pseudo‑random number generator (PRNG) with a lock‑free entropy pool, designed primarily for **operating system kernels, embedded systems, and IoT environments**.

Despite its historical naming and structural inspiration, RC4OK is **not intended to be used as a stream cipher for encryption**. Its design goals, threat model, and engineering trade‑offs are fundamentally different from those of modern cryptographic ciphers.

---

## Design Goals

RC4OK was developed to address practical problems of randomness generation inside operating systems:

* Fast generation of high‑quality random streams
* Lock‑free and low‑latency entropy mixing from asynchronous physical events
* Safe operation in multiprocessor environments
* Minimal internal state and low computational overhead

Typical entropy sources include device interrupts, timing jitter, I/O events, and other non‑deterministic physical signals.

---

## What RC4OK *Is*

* A **strong PRNG** suitable for OS kernels and low‑level system components
* An **entropy sink** that safely absorbs asynchronous physical events
* A generator optimized for **performance, robustness, and simplicity**

The output stream demonstrates high statistical quality and successfully passes large‑scale empirical randomness tests (e.g., PractRand), making it suitable for non‑blocking system randomness needs.

---

## What RC4OK *Is Not*

* ❌ Not a general‑purpose stream cipher
* ❌ Not a replacement for AES, ChaCha20, or other modern encryption primitives
* ❌ Not a design with formal cryptographic proofs

RC4OK should **not** be used directly for data encryption or as a drop‑in cryptographic primitive in adversarial protocols.

---

## Delayed Entropy Influence: Feature, Not a Bug

A frequently discussed property of RC4OK is that **injected entropy does not immediately affect the output stream**.

This behavior is **intentional** and plays a critical role in the security model:

* Prevents synchronous attacks that attempt to control physical entropy sources (e.g., deliberate device activity)
* Breaks temporal correlation between external events and generated output
* Ensures that entropy is *absorbed, diffused, and internalized* before influencing output

In the context of OS‑level randomness, immediate reflection of entropy can be a liability. RC4OK deliberately introduces diffusion latency to protect against such scenarios.

---

## Relation to RC4

RC4OK borrows conceptual elements from the classical RC4 algorithm (such as a dynamically permuted state array), but significantly modifies:

* State evolution
* Indexing behavior
* Entropy mixing mechanisms

While the associated publication uses the phrase *"an improved version of the classical RC4 stream cipher"*, the same abstract explicitly states the intended purpose:

> *"The improvements allow to build lightweight high‑performance cryptographically strong random number generator suitable for use in IoT and as a corresponding component of operating systems."*

RC4OK should therefore be understood as an **RC4‑inspired PRNG**, not as an encryption algorithm.

---

## Publication

The design and rationale of RC4OK are described in the following paper:

**RC4OK. An improvement of the RC4 stream cipher**
Springer Link: [https://link.springer.com/article/10.1007/s11416-025-00548-2](https://link.springer.com/article/10.1007/s11416-025-00548-2)

The paper focuses on engineering considerations for system‑level randomness, performance, and entropy handling rather than on formal cryptographic proofs.

---

## Intended Use Cases

* Operating system kernels
* Embedded systems and firmware
* IoT devices
* High‑performance system services requiring non‑blocking randomness

---

## Summary

RC4OK is a pragmatic, system‑oriented PRNG designed for environments where **performance, simplicity, and safe entropy handling matter more than cryptographic formalism**.

If you are looking for a fast, lock‑free RNG for system software — RC4OK fits.
If you are looking for an encryption algorithm — use a modern, well‑analyzed cipher instead.

