## CacheSim – Trace‑Driven CPU Cache Simulator (C++)

 **CPU cache simulator** written in modern **C++20**, designed to help build intuition for cache behavior and cache‑aware programming.

- **Technologies**: C++20, standard library only, `Makefile` build.
- **Use cases**: Experiment with cache sizes/associativity, replacement policies, and access patterns; analyze hit/miss rates and memory traffic from synthetic or real traces.

---

## Features

- **Trace‑driven simulation**
  - Reads a simple text trace (`.cmt`) of memory operations:
    - One instruction per line: `<R|W> <32‑bit hex address>`, for example: `R 0x00004000`.

- **Configurable cache organization**
  - Set‑associative cache with:
    - Line size: **64 bytes**.
    - Total size: **8 KB – 64 KB** (via CLI).
    - Associativity: **1–1024 ways** (power of two).
  - 32‑bit addresses, 4‑byte alignment, 4 MiB main memory.

- **Policies**
  - **Replacement**: Least Recently Used (**LRU**) or Random Replacement (**RR**).
  - **Write policy**: **Write‑through**.
  - **Allocation**: **No‑write‑allocate**.

- **Statistics and reporting**
  - High‑level stats: instruction count, hit/miss counts and rates, bytes transferred between cache and main memory.
  - Optional per‑instruction dump including type, address, tag, set index, byte offset, result (hit/miss), evictions, etc.

---

## Project Layout

- `src/`
  - `Cache.h/.cpp`: Cache sets/lines, tag lookup, replacement, statistics.
  - `MainMemory.h/.cpp`: Simple 4 MiB byte‑addressable main memory.
  - `Processor.h/.cpp`: `MemorySystem` wrapper that ties cache and main memory together.
  - `ReplacementAlgorithms.h/.cpp`: Random replacement policy helper.
- `traces/`
  - Example `.cmt` traces:
    - `cache_trace_best_case.cmt`
    - `cache_trace_common_case.cmt`
    - `cache_trace_worst_case.cmt`
- `Makefile`
  - Builds the simulator into the `CacheSim` binary and provides a convenience `run` target.

---

## Building

You just need a reasonably recent **GCC** or **Clang** with C++20 support.

```bash
cd CacheSim
make
```

This produces the `CacheSim` executable in the repository root.

Clean build artifacts:

```bash
make clean
```

---

## Usage

The simulator is driven by a trace file and a few command‑line parameters:

```bash
./CacheSim <path_to_tracefile.cmt> <cache_size_kb> <associativity> <replacement_algorithm> <output_level>
```

- `path_to_tracefile.cmt`  
  Path to a `.cmt` file containing instructions like `R 0x00000000` or `W 0x00001000`.

- `cache_size_kb`  
  Total cache capacity in kilobytes. Allowed values: `8`, `16`, `32`, `64`.

- `associativity`  
  Number of ways per set (must be a power of two, between `1` and `1024`).

- `replacement_algorithm`  
  - `LRU` – Least Recently Used.  
  - `RR`  – Random Replacement.

- `output_level`  
  - `0` – Minimal: only aggregate statistics.  
  - `1` – Extended: per‑instruction statistics.

### Examples

Run the common‑case trace with a 32 KB, 4‑way LRU cache and detailed output:

```bash
./CacheSim traces/cache_trace_common_case.cmt 32 4 LRU 1
```

Run a best‑case trace with a smaller cache and minimal stats:

```bash
./CacheSim traces/cache_trace_best_case.cmt 16 2 RR 0
```

---

## Trace Format

Trace files are plain text and easy to generate from other tools or benchmarks.

Each line has:

```text
<op> <address>
```

Where:

- `<op>` is `R` (read) or `W` (write).
- `<address>` is a 32‑bit byte address written as `0x`‑prefixed hex.

Example (`traces/cache_trace_best_case.cmt`):

```text
R 0x00004000
R 0x00004004
R 0x00004008
```

---

## How It Works (High‑Level)

At a high level:

1. The main loop parses each trace instruction and calls `MemorySystem::Read` or `MemorySystem::Write`.
2. `MemorySystem` first consults the `Cache`:
   - On a **hit**, the access is served directly from a `CacheLine`.
   - On a **miss**, the relevant line is fetched from `MainMemory`, installed into the set (using LRU or RR), and then accessed.
3. **Write‑through / no‑write‑allocate**:
   - Writes always update main memory.
   - Writes that miss in the cache do not allocate a new line.
4. `CacheStatistics` tracks access and hit counts, from which hit rate and derived metrics are reported.

The code is intentionally small and modular so it can be read easily.

---

## Extending the Simulator

Ideas for further exploration:

- Implement additional replacement policies (e.g., pseudo‑LRU, FIFO, LFU).
- Add configurable **block size** and **number of sets/ways** instead of compile‑time constants.
- Model multi‑level caches (L1 + L2) and measure combined hit rates.
- Add CSV/JSON exporters for statistics to plug into Jupyter notebooks or plotting tools.
- Integrate real traces from benchmarks, compilers, or instruction‑set simulators.


