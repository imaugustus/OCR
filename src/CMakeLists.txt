cmake_minimum_required(VERSION 2.8)
add_library(src 
	ChunkArray.cpp
	EditDistance.cpp
	SuggestionStage.cpp
	SuggestItem.cpp
	SymSpellCheck.cpp
	VTM.cpp
)	
add_executable(Main main.cpp)


