# Vulkan Fast Fourier Transform
This library can calculate a multidimensional [Discrete Fourier Transform](https://en.wikipedia.org/wiki/Discrete_Fourier_transform) on the GPU using the [Vulkan API](https://www.khronos.org/vulkan/).
However, in most cases you probably want a different library,
because Vulkan does not change much about the GPU computations,
but is a lot more complex than other APIs.
Alternatives are based on [OpenGL](https://github.com/Themaister/GLFFT), [OpenCL](https://gpuopen.com/compute-product/clfft/) or [CUDA](https://developer.nvidia.com/cufft) for example.
Some reasons to use this library anyway are:
- You already have a Vulkan application and just need a FFT implementation
- You want to reduce CPU load, because Vulkan is meant do exactly that
- You are on a platform where other options are worse or just not supported (e.g. OpenGL on MacOS)
- You are just here for the CLI and don't care about Vulkan

## Command Line Interface
Note that many small invocations are very inefficient, because the startup costs of Vulkan are very high.
So the CLI is only useful for transforming big data sets and testing.

```bash
VK_SDK=path/to/vulkan/installation
mkdir build && cd build
cmake .. -DVK_SDK=$VK_SDK
make
export VK_ICD_FILENAMES=$VK_SDK/etc/vulkan/icd.d/driver_icd.json
export VK_LAYER_PATH=$VK_SDK/etc/vulkan/explicit_layer.d
```

### Options
- `-x width` Samples in x direction
- `-y height` Samples in y direction
- `-z depth` Samples in z direction
- `--inverse` Calculate the IDFT
- `--input raw / ascii / png` Input encoding
- `--output raw / ascii / png` Output encoding
- `--device index` Vulkan device to use
- `--list-devices` List Vulkan devices

### Example Invocations
```bash
vulkanfft -x 16 -y 16 --input ascii --output png --inverse < test.txt > test.png
vulkanfft -x 16 -y 16 --input png --output ascii < test.png
```

## Dependencies
- cmake 3.13
- libpng 1.6 (optional, only needed for CLI)
- hexdump (to inline SPIR-V in C)
- Vulkan SDK 1.1.97 (to compile GLSL to SPIR-V)
- Vulkan Runtime 1.0

## Current Features & Limitations
- Dimensions
    - Only 1D, 2D, 3D
    - No higher dimensions
- Direction & Scale
    - Forward / backward (inverse)
    - No normalized / unnormalized switch independent of direction
- Sample Count / Size
    - Only POT (power of two)
    - No prime factorization
- Memory Layout
    - Only buffers (row-major order)
    - No samplers / images / textures (2D tiling)
    - Only interleaved complex float
    - No separation of real and imaginary parts
- Bit-Depth & Data Types
    - Only 32 bit complex floats
    - No real only mode
    - No 8, 16, 64, 128 bit floats or integers
- Parallelization / SIMD
    - Only radix 2
    - No higher radix
- Memory Requirements
    - 2*n because of swap buffers for Stockham auto-sort algorithm
    - No reordering and in-place operation
- Memorization & Profiling
    - Only cold planning
    - No memorization or warm planning / wisdom profiling
