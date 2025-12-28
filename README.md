# RGSL - RaeptorCogs Shading Language

<!-- markdownlint-disable MD033 -->
<p align="center">
    <img src="rgsl-logo.png" alt="RGSL logo" style="width: 20%" />
    <br><br>
    <strong>RGSL (RaeptorCogs Shading Language)</strong> is a high-level
    <br>shading language designed for the RaeptorCogs graphics framework.
    <br><br>
</p>
<!-- markdownlint-disable MD033 -->

## RGSL Overview

**RGSL** is a domain-specific language tailored for writing shaders within the RaeptorCogs graphics framework. It provides a simplified syntax and semantics for defining vertex and fragment shaders, making it easier for developers to create complex visual effects without delving into the intricacies of GLSL or HLSL.
Is mainly designed to unify shader development into a single language that can be transpiled to multiple shading languages, facilitating cross-platform graphics programming.
It also let developers use GLSL directly when needed.

## Features

- High-level abstractions for common shader operations
- Support for vertex and fragment shaders

## Roadmap

- [ ] Complete RGSL language specification
- [ ] Implement RGSL to GLSL transpiler
- [ ] Integrate RGSL into RaeptorCogs framework
- [ ] Develop documentation and tutorials
- [ ] Community feedback and iteration
- [ ] Support for additional shading languages (e.g., HLSL, Metal)

## Building

### Requirements

- C++17 or newer
- CMake 3.16 or newer

### Desktop build

```bash
git clone https://github.com/Estorc/RGSL.git
cd RGSL
mkdir build && cd build
cmake ..
cmake --build .
```

## License

RaeptorCogs is licensed under the MIT License.
See the [**LICENSE**](https://github.com/Estorc/RaeptorCogs/blob/main/LICENSE.txt) file for details.
