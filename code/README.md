To generate input_cases, compile Graph.java using the command:
$ javac Graph.java
To get the input file, run:
$ java Graph <width> <height> <input_case_file_name>
(where width and height are the dimensions of the graph and input_case_file_name is the name of the input case file.)
To run the project, use the command :
$ g++ -fopenmp -o <output> query.cpp sequential.cpp hda.cpp -ltbb
$ <output>
(where output is the name of the compiled executable program)
