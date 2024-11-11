vertShaderSrc := src/shader.vert
fragShaderSrc := src/shader.frag
vertShaderOut := out/vert.spv
fragShaderOut := out/frag.spv

compiler := glslc

all: shader

shader:
	${compiler} ${vertShaderSrc} -o ${vertShaderOut}
	${compiler} ${fragShaderSrc} -o ${fragShaderOut}
