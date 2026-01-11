#include <RGSL/external/glslang_c.h>

#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>

#include <vector>
#include <string>

#ifdef WIN32
#define strdup _strdup
#endif

static TBuiltInResource InitResources()
{
    TBuiltInResource Resources;

    Resources.maxLights                                 = 32;
    Resources.maxClipPlanes                             = 6;
    Resources.maxTextureUnits                           = 32;
    Resources.maxTextureCoords                          = 32;
    Resources.maxVertexAttribs                          = 64;
    Resources.maxVertexUniformComponents                = 4096;
    Resources.maxVaryingFloats                          = 64;
    Resources.maxVertexTextureImageUnits                = 32;
    Resources.maxCombinedTextureImageUnits              = 80;
    Resources.maxTextureImageUnits                      = 32;
    Resources.maxFragmentUniformComponents              = 4096;
    Resources.maxDrawBuffers                            = 32;
    Resources.maxVertexUniformVectors                   = 128;
    Resources.maxVaryingVectors                         = 8;
    Resources.maxFragmentUniformVectors                 = 16;
    Resources.maxVertexOutputVectors                    = 16;
    Resources.maxFragmentInputVectors                   = 15;
    Resources.minProgramTexelOffset                     = -8;
    Resources.maxProgramTexelOffset                     = 7;
    Resources.maxClipDistances                          = 8;
    Resources.maxComputeWorkGroupCountX                 = 65535;
    Resources.maxComputeWorkGroupCountY                 = 65535;
    Resources.maxComputeWorkGroupCountZ                 = 65535;
    Resources.maxComputeWorkGroupSizeX                  = 1024;
    Resources.maxComputeWorkGroupSizeY                  = 1024;
    Resources.maxComputeWorkGroupSizeZ                  = 64;
    Resources.maxComputeUniformComponents               = 1024;
    Resources.maxComputeTextureImageUnits               = 16;
    Resources.maxComputeImageUniforms                   = 8;
    Resources.maxComputeAtomicCounters                  = 8;
    Resources.maxComputeAtomicCounterBuffers            = 1;
    Resources.maxVaryingComponents                      = 60;
    Resources.maxVertexOutputComponents                 = 64;
    Resources.maxGeometryInputComponents                = 64;
    Resources.maxGeometryOutputComponents               = 128;
    Resources.maxFragmentInputComponents                = 128;
    Resources.maxImageUnits                             = 8;
    Resources.maxCombinedImageUnitsAndFragmentOutputs   = 8;
    Resources.maxCombinedShaderOutputResources          = 8;
    Resources.maxImageSamples                           = 0;
    Resources.maxVertexImageUniforms                    = 0;
    Resources.maxTessControlImageUniforms               = 0;
    Resources.maxTessEvaluationImageUniforms            = 0;
    Resources.maxGeometryImageUniforms                  = 0;
    Resources.maxFragmentImageUniforms                  = 8;
    Resources.maxCombinedImageUniforms                  = 8;
    Resources.maxGeometryTextureImageUnits              = 16;
    Resources.maxGeometryOutputVertices                 = 256;
    Resources.maxGeometryTotalOutputComponents          = 1024;
    Resources.maxGeometryUniformComponents              = 1024;
    Resources.maxGeometryVaryingComponents              = 64;
    Resources.maxTessControlInputComponents             = 128;
    Resources.maxTessControlOutputComponents            = 128;
    Resources.maxTessControlTextureImageUnits           = 16;
    Resources.maxTessControlUniformComponents           = 1024;
    Resources.maxTessControlTotalOutputComponents       = 4096;
    Resources.maxTessEvaluationInputComponents          = 128;
    Resources.maxTessEvaluationOutputComponents         = 128;
    Resources.maxTessEvaluationTextureImageUnits        = 16;
    Resources.maxTessEvaluationUniformComponents        = 1024;
    Resources.maxTessPatchComponents                    = 120;
    Resources.maxPatchVertices                          = 32;
    Resources.maxTessGenLevel                           = 64;
    Resources.maxViewports                              = 16;
    Resources.maxVertexAtomicCounters                   = 0;
    Resources.maxTessControlAtomicCounters              = 0;
    Resources.maxTessEvaluationAtomicCounters           = 0;
    Resources.maxGeometryAtomicCounters                 = 0;
    Resources.maxFragmentAtomicCounters                 = 8;
    Resources.maxCombinedAtomicCounters                 = 8;
    Resources.maxAtomicCounterBindings                  = 1;
    Resources.maxVertexAtomicCounterBuffers             = 0;
    Resources.maxTessControlAtomicCounterBuffers        = 0;
    Resources.maxTessEvaluationAtomicCounterBuffers     = 0;
    Resources.maxGeometryAtomicCounterBuffers           = 0;
    Resources.maxFragmentAtomicCounterBuffers           = 1;
    Resources.maxCombinedAtomicCounterBuffers           = 1;
    Resources.maxAtomicCounterBufferSize                = 16384;
    Resources.maxTransformFeedbackBuffers               = 4;
    Resources.maxTransformFeedbackInterleavedComponents = 64;
    Resources.maxCullDistances                          = 8;
    Resources.maxCombinedClipAndCullDistances           = 8;
    Resources.maxSamples                                = 4;
    Resources.maxMeshOutputVerticesNV                   = 256;
    Resources.maxMeshOutputPrimitivesNV                 = 512;
    Resources.maxMeshWorkGroupSizeX_NV                  = 32;
    Resources.maxMeshWorkGroupSizeY_NV                  = 1;
    Resources.maxMeshWorkGroupSizeZ_NV                  = 1;
    Resources.maxTaskWorkGroupSizeX_NV                  = 32;
    Resources.maxTaskWorkGroupSizeY_NV                  = 1;
    Resources.maxTaskWorkGroupSizeZ_NV                  = 1;
    Resources.maxMeshViewCountNV                        = 4;

    Resources.limits.nonInductiveForLoops                 = 1;
    Resources.limits.whileLoops                           = 1;
    Resources.limits.doWhileLoops                         = 1;
    Resources.limits.generalUniformIndexing               = 1;
    Resources.limits.generalAttributeMatrixVectorIndexing = 1;
    Resources.limits.generalVaryingIndexing               = 1;
    Resources.limits.generalSamplerIndexing               = 1;
    Resources.limits.generalVariableIndexing              = 1;
    Resources.limits.generalConstantMatrixVectorIndexing  = 1;

    return Resources;
}

static EShLanguage StageFromString(const char* s) {
    if (!strcmp(s, "vert")) return EShLangVertex;
    if (!strcmp(s, "frag")) return EShLangFragment;
    if (!strcmp(s, "geom")) return EShLangGeometry;
    if (!strcmp(s, "tesc")) return EShLangTessControl;
    if (!strcmp(s, "tese")) return EShLangTessEvaluation;
    if (!strcmp(s, "comp")) return EShLangCompute;
    return EShLangCount;
}

void rgsl_glslang_initialize() {
    glslang::InitializeProcess();
}

void rgsl_glslang_finalize() {
    glslang::FinalizeProcess();
}

bool rgsl_glslang_validate_glsl(const char* source, const char* stage_str, char** out_log) {
    EShLanguage stage = StageFromString(stage_str);
    if (stage == EShLangCount) {
        *out_log = strdup("Invalid shader stage specified.");
        return false;
    }

    glslang::TShader shader(stage);
    shader.setStrings(&source, 1);

    EShMessages messages = EShMsgDefault;
    TBuiltInResource resources = InitResources();
    if (!shader.parse(&resources, 100, false, messages)) {
        *out_log = strdup(shader.getInfoLog());
        return false;
    }

    glslang::TProgram program;
    program.addShader(&shader);
    if (!program.link(messages)) {
        *out_log = strdup(program.getInfoLog());
        return false;
    }

    *out_log = strdup(program.getInfoLog());
    return true;
}

struct rgsl_glslang_result rgsl_glslang_compile_glsl(const char* source, const char* stage_str) {
    struct rgsl_glslang_result result = {};
    EShLanguage stage = StageFromString(stage_str);
    if (stage == EShLangCount) {
        result.log = strdup("Invalid shader stage specified.");
        result.success = 0;
        return result;
    }

    glslang::TShader shader(stage);
    shader.setStrings(&source, 1);

    EShMessages messages = EShMsgDefault;
    TBuiltInResource resources = InitResources();
    if (!shader.parse(&resources, 100, false, messages)) {
        result.log = strdup(shader.getInfoLog());
        result.success = 0;
        return result;
    }

    glslang::TProgram program;
    program.addShader(&shader);
    if (!program.link(messages)) {
        result.log = strdup(program.getInfoLog());
        result.success = 0;
        return result;
    }

    glslang::TIntermediate* intermediate = program.getIntermediate(stage);
    if (!intermediate) {
        result.log = strdup("Failed to get intermediate representation.");
        result.success = 0;
        return result;
    }

    std::vector<unsigned int> spirv;
    GlslangToSpv(*intermediate, spirv);

    size_t word_count = spirv.size();
    uint32_t* words = (uint32_t*)malloc(word_count * sizeof(uint32_t));
    memcpy(words, spirv.data(), word_count * sizeof(uint32_t));

    result.words = words;
    result.word_count = word_count;
    result.log = strdup(program.getInfoLog());
    result.success = 1;
    return result;
}

void rgsl_glslang_free_result(struct rgsl_glslang_result* r) {
    free((void*)r->words);
    free((void*)r->log);
}
