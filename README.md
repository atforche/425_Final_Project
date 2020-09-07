# 425_Final_Project

This project creates a simulation that closely mirrors the simulation found in the Jyro Python Robot Simulation (https://jyro.readthedocs.io/en/latest/Jyro%20Simulator.html). On top of the simulation is a neural network framework that allows the robots in the simulation to be controlled by a neural network that takes in certain sensor readings and camera images from the environment, depending on the specified mode. In addition to training these neural networks, this code also allows for the replay of the best neural networks from each generation in the same simulation environment to allow for the visualization of the predator-prey coevolution occuring in the system. This project is built upon the OpenFrameworks toolkit (https://openframeworks.cc/), so building and modifying the code requires the OpenFrameworks dependencies. 

# Table Of Contents

*[Tournament](#tournament)

*[Simulation](#simulation)

*[Neural_Net](#neural_net)

*[Robot](#robot)

*[Sensor](#sensor)

*[Wall](#wall)

*[Coordinate](#coordinate)

*[Helpers](#helpers)

# <a name="tournament"></a> 
# Tournament
Class that contains all of the members and functions necessary for the execution of the tournament training process for the neural networks
#### Tournament()
Default constructor for the Tournament class
#### Tournament(int pop_size_in, int num_generation_in)
Custom constructor for the Tournament class that creates a tournament with the specified population_sizes and number of generations
#### Set_Mode(int mode_in)
Sets the input that the neural network will take in from the environment. Options include Position, Sensors, Image, and Image_and_Sensors, which are specified in the enum in ofApp.cpp
#### Set_Mutation_Rate(double mutation_rate_in)
Sets the mutation rate of the neural_nets in the tournament
#### Initialize_Population(int num_inputs_in, std::vector<int> &hidden_layers_in)
Initializes a prey and predator population of neural networks of size population size that take in num_inputs_in inputs and has a hidden node count equal to hidden_layers_in
#### Add_Robot(Robot &robot_in)
Adds a new robot into the simulation being run by the tournament
#### Initialize(int num_robots)
Initializes the simulation and the predator and prey populations of robots. There are num_robots predators and num_robots prey
#### Render(bool render = false)
Renders all of the robots and walls in the simulation. Optional bool argument specifies whether simulation will be rendered on every iteration or only after num_generations have passed
#### Run_Tournament(bool render = false)
Evaluates every robot in one population against its corresponding robot in the opposite population. Evolves one of the predator or prey population at a time. Swaps which one is being evolved after num_generations have passed. Performs tournament selection on the populations. Writes the best predator and prey from each cycle into a file
#### std::vector<Neural_Net> Tournament_Selection(int tournament_size)
Performs tournament selection on the evolving population and returns a vector that contains the next generation
#### Calculate_Pred_Fitness(Robot* robot, int index)
Calculates the fitness of the inputted robot. Pred fitness is defined as the max_distance minus the robots current distance from its corresponding prey
#### Calculate_Prey_Fitness(Robot* robot, int index)
Calculates the fitness of the inputted robot. Prey fitness is defined as the current distance between the robot and its corresponding predator
#### Select_Pred_Population(std::vector<Neural_Net> &pred_population, std::vector<double> &pred_fitnesses)
Performs tournament selection on the pred_population according to the pred_fitnesses. Modifies the pred_population vector with the new population and clears the pred_fitnesses vector
#### Select_Prey_Population(std::vector<Neural_Net> &prey_population, std::vector<double> &prey_fitnesses)
Performs tournament selection on the prey_population according to the prey_fitnesses. Modifies the prey_population vector with the new population and clears the prey_fitnesses vector
#### Get_Generation()
Returns the current generation that the tournament is on
#### Swap_Population()
Swaps the population that is being evolved. Resets the robots positions to the default positions for a pred/prey
#### Reset_and_Select_Population(std::vector<Robot*> &robots)
Resets all of the robots to their default positions. Performs fitness sharing on the Neural_Networks. Then calls Selection_Pred_Population/Select_Prey_Population to perform tournament selection and get the next generation
#### Evaluate_Evolving_Population(std::vector<Robot*> &robots)
Executes the neural networks according to the mode that is selected. Then moves the robots according to the network's execution. Then, evaluates the fitness of each individual in the evolving population and updates the appriopriate fitness vector
#### Evaluation_Static_Population(std::vetor<Robot*> &robots)
Executes the neural networks according to the mode that is selected. Then moves the robots accordint to the network's execution. No fitness is requires as this evaluates the non-evolving population
#### Simulate_Brains_Initialize()
Initializes the simulation with the settings that allow neural networks from a file to be simulation
#### Simulate_Brains()
Runs the simulation with a single predator and prey that are initialized from files specified in the function
#### Test()
General tester function for features that does not require that the tournament be run
#### ~Tournament()
Destructor for the tournament object. Cleans up all dynamic memory allocations

# <a name="simulation"></a>
# Simulation
Class that contains all of the members and functions necessary to running the simulation based upon the Jyro Python Robot Simulator
#### Sim()
Creates a default simulation with width 800 and height 800
#### Sim(double window_width_in, double window_height_in)
Creates a custom simulation with width and height equal to the input
#### Add_Robot(Robot &robot)
Adds a new robot into the simulation
#### Add_Walls(Wall& wall)
Adds a new wall into the simulation
#### Render(bool sensors = false)
Renders all of the robots and walls in the simulation. Optional argument specifies whether or not to render each of the sensors for each robot
#### IsHalted()
Returns whether or not the simulation is halted
#### Initialize()
Adds four walls around the boundary of the simulation
#### GetWalls()
Returns a std::vector<Wall> containing all of the wall objects in the simulation
#### GetRobots()
Returns a std::vector<Robot*> containing all of the robot objects in the simulation
#### Reset()
Deletes all of the robots in the simulation and clears the robots vector
#### ~Sim()
Destructor for the Sim object. Cleans up all dynamically allocated memory

# <a name="neural_net"></a>
# Neural_Net
Class that contains all the members and functions requires to execute a neural network
#### Neural_Net(int num_inputs_in, int num_outputs_in, std::vector<int> &layer_node_count_in = std::vector<int>())
Creates a Neural_Net that takes in num_inputs inputs and produces num_outputs outputs. Optional argument layer_node_count_in specifies the number of hidden layers and the number of hidden nodes at each layer. Created network has random weights between -1 and 1.
#### Activation_Function(double x)
Calculates the activation fucntion of a node given an input x. The function used is the hyperbolic tangent function
#### Deep_Copy()
Produces a deep copy of the neural network
#### Execute(std::vector<double> &input_vector)
Executes the Neural_Net according to the input vector
#### Mutation_Network(double mutation_rate)
Mutates each weight in the network with probability mutation_rate. The magnitude of the mutation is a random real number between -1 and 1
#### Get_Layers()
Returns a std::vector<std::vector<std::vector<double>>> that specifies all of the layers in the network
#### Print(ofstream &outfile)
Prints out the Neural_Net into the specified ofstream
#### Read(ifstream &infile)
Overwrites the Neural_Net object with the Neural_Net specified in the ifstream

# <a name="robot"></a>
# Robot
Class that contains all the members and function for the robots that operate within the simulation
#### Robot()
Creates a default robot object
#### Robot(double x_in, double y_in, double orientation_in)
Creates a custom robot at position (x_in, y_in) with the specified orientation
#### SetSpeed(int speed_in)
Sets the maximum speed of the robot
#### SetWalls(std::vector<Wall> &wall_in)
Sets the walls that the robot and its sensors will be able to detect
#### AddSensor(Sensor &sensor)
Adds another sensor object to the robot
#### SetSprite(ofImage sprite_in)
Sets the sprite of the robot to an ofImage
#### SetPosition(Coordinate position_in, double rotation_in)
Sets the position of the robot to position_in, and sets the rotation of the robot to rotation_in
#### Render(bool render_sensors = false)
Renders the robot and all of its sensors. Optional argument specifies whether or not to render the sensors
#### Move(double input)
Input is a value between -1 and 1. Tells the robot to move forward or backward by some fraction of its max speed
#### Rotate(double input)
Input is a value between -1 and 1. Tells the robot to rotate to the left or right by some fraction times 15 degrees
#### Collided(double input = 0, Robot* robot = NULL)
Returns whether or not the robot has collided. Optional argument input allows the fuction to check if the robot will collided if it carries out the specified move. Optional argment robot allows the function to check if the robot has collided with another robot
#### Initialize()
Initialize the eleven sensors located around the robot
#### GetSensorReading(int index)
Returns the distance reading from the sensor specified by the index
#### GetSensorReadings()
Returns a std::vector<double> that contains the distance readings for all of the sensors on the robot
#### Get_Position()
Returns the position of the robot as a Coordinate
#### Get_Orientation()
Returns the orientation of the robot as a double
#### Get_Camera_Output(std::vector<Robot*> robots)
Returns a std::vector<std::vector<ofColor>> that is the camera output from the robots camera module. The argument specifies the list of other robots that the robot is capable of seeing
#### Get_Sprite_Width()
Returns the width of the robots sprite
#### Get_Color()
Returns a reference to the color of the robot
#### Set_Pred(bool pred_in)
Sets whether or not the robot is a predator
#### Get_Pred()
Returns whether or not the robot is a predator
#### Get_Collided()
Returns the collided flag of the robot
#### ~Robot()
Destructor for the robot class. Cleans up all dynamically allocated memory

# <a name="sensor"></a>
# Sensor
Class that contains all the members and functions for the distance sensors attached to the robots
#### Sensor()
Creates a default sensor object
#### Sensor(double x_in, double y_in, double rotation_in)
Creates a sensor object at position (x_in, y_in), with an orientation equal to rotation_in
#### AddWalls(std::vector<Wall> &walls_in)
Adds another wall to the list of walls that the sensor is capable of detecting
#### Render()
Renders the sensor object as a line
#### Calculate_Distance()
Calculates the distance between the sensor and the nearest wall that it intersects. Formula comes from https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
#### Camera_Calculate_Distance(ofColor &color_in)
Calculates the distance between the sensor and the nearest object that it intersects. Modifies color_in to contain the color of the object that is first interesected by the sensor. Returns the distance as a double
#### Wall_Collision(double min_dist)
Takes in the distance between the sensor and the nearest wall and uses that to calculate the Coordinate of the intersection between the sensor and the wall
#### GetX()
Returns a reference to the x position of the sensor
#### GetY()
Returns a referece to the y position of the sensor
#### GetRotation()
Returns a reference to the rotation property of the sensor
#### GetOrientation()
Returns a reference to the orientation property of the sensor
#### Rotate(double rotation_in)
Rotates the sensor by rotation_in number of degrees

# <a name="wall"></a>
# Wall
Contains all the members and functions for the walls of the simulation
#### Wall()
Creates a default wall from (0,0) to (0,0)
#### Wall(Coordinate begin, Coordinate end)
Creates a wall that goes from (begin.x, begin.y) to (end.x, end.y)
#### Render()
Renders the wall object in the simulation
#### GetBeginning()
Returns the beginning of the wall as a Coordinate
#### GetEnding()
Returns the ending of the wall as a Coordinate
#### Get_Color()
Returns a reference to the color of the wall

# <a name="coordinate"> </a>
# Coordinate
Class that contains all the members and functions for the Coordinate class in the simulation
#### Coordinate()
Creates a default Coordinate at (0,0)
#### Coordinate(double x_in, double y_in)
Creates a custom Coordinate at (x_in, y_in)
#### GetX()
Returns a reference to the x position of the Coordinate
#### GetY()
Returns a reference to the y position of the Coordinate
#### Cross_Product(Coordinate one, Coordinate two)
Returns the cross product between two Coordinates, one cross two
#### Dot_Product(Coordinate one, Coordinate two)
Returns the dot product between two Coordinates
#### Distance(Coordinate one, Coordinate two)
Returns the Euclidean distance between the Coordinates
#### Subtract(Coordinate one, Coordinate two)
Returns a Coordinate that is the difference between one and two (one - two)

# <a name="helpers"></a>
# Helpers
Header that contains all of the various helper functions needed in the project
#### Matrix_Multiplication(std::vector<std::vector<double>> &one, std::vector<std::vector<double>> &two)
Returns a 2D vector that is the result of matrix multiplication between one and two
#### Map_Distance_To_Pixels(double distance_readings)
Returns the number of pixels in the camera image a color should take up if it is distance_reading distance away from the camera
#### Print_Image(std::vector<std::vector<ofColor>> &image)
Prints the Image to the console
#### Create_Image(std::vector<std::vector<ofColor>> &image)
Returns an ofImage constructed from the 2D vector of colors
#### Convolute_Image(std::vector<std::vector<ofColor>> &image)
Returns a 2D vector of doubles that is the results of a convolution operation on the image. The filters are chosen with the purpose of detecting a rectangular region in the image. The size of the returns 2D vector is 1/3 the size of the original image because the filters are only applied over every third pixel
#### Column_Pooling(std::vector<std::vector<double>> &image)
Converts the 2D vector containing the convoluted image into a 1D vector that represents the average intensity value in each column of the image
#### Map_Values(std::vector<double> &pooled_input)
Maps the values in a vector from the range (min(pooled_input), max(pooled_input)) to the range(0,1). Modified the inputted vector. Calculates specified in https://rosettacode.org/wiki/Map_range.
#### Neural_Net_Distance(std::vector<std::vector<std::vector<double>>> x_layers, std::vector<std::vector<std::vector>>> y_layers)
Returns how similar the two neural networks are based on the euclidean distance between their layers
#### Neural_Net_Sharing(Neural_Net* a, Neural_Net* b)
Returns the sharing factor between two neural networks. This factor is then multiplied by the fitness of a to reduce it according to fitness sharing
#### Color_Filter(std::vector<std::vector<ofColor>> &camera_output, bool predator)
Performs a color filter operation on the image. If the robot is a predator, then blue is the only color preserved. If the robot is a prey, then ren is the only color perserved
