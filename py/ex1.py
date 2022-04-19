import sys

class Env:
    """ Class for global variables used in the system """
    k = "k"
    input_file = "input_file"
    output_file = "output_file"
    maxiter = "maxiter"

class Centroid:
    """
        Class represents a centroid of k-means algorithm 
        A cenroid is represented by the sum of all vectors assigned to the centroid
        and the number of vectors as count
    """
    def __init__(self, vector: float = None):
        if vector is None:
            self.vec_sum = []
            self.count = 0
        else:
            self.vec_sum = vector
            self.count = 1
    
    def __str__(self) -> str:

        return str([self[i] for i in range(len(self))])

    
    def __len__(self) -> int:
        return len(self.vec_sum)

    def __getitem__(self, i: int) -> float:
        return self.vec_sum[i]/self.count

    def add_vector(self, vector: list[float]):
        if self.count == 0:
            self.vec_sum = vector
        else:
            for i in range(len(self.vec_sum)):
                self.vec_sum[i] += vector[i]
                
        self.count += 1
    
    def distance(self, vector) -> float:
        dist = 0
        for i in range(len(vector)):
            dist += ( self[i] - vector[i] )**2
        return dist**0.5
    
    def max_distance(centroids1, centroids2):
        maxd = float('-inf')
        for i in range(len(centroids1)):
            maxd = max(maxd, centroids1[i].distance(centroids2[i]))
        return maxd
    

def main() -> int:
    try:
        args = load_args()
    except Exception as ex:
        print(ex)
        return
    
    try:#check
        centroids = initialize_centroids(args[Env.k], args[Env.input_file])
    except Exception as ex:
        print(ex)
        return
    
    maxiter = args.get(Env.maxiter, float('inf'))
    i=0
    while i < maxiter:
        try:
            new_centroids = kmeans_iteration(centroids, args[Env.input_file])
        except Exception as ex:
            print(ex)

        if Centroid.max_distance(centroids, new_centroids) < 0.001:
            break
        else:
            centroids = new_centroids
            i+=1
    
    write_output(args[Env.output_file], new_centroids)
            
def create_vector_from_line(line: str):
    return [float(x) for x in line.split(',')]


def initialize_centroids(k: int, input_name: str):
    try:
        f = open(input_name, mode="r")
    except:
        raise Exception("An Error Has Occurred")


    centroids = []

    while (len(centroids) < k):
        line = f.readline()
        if line == '\n':
            break
        
        #Validating all centorids are valid floats
        try:
            centroid = Centroid(create_vector_from_line(line))
        except:
            raise Exception("Invalid Input!")
        
        #Validating all centroids are of same length
        if len(centroids)>0 and len(centroid) != len(centroids[0]):
            raise Exception("Invalid Input!")
        
        centroids.append(centroid)
    
    for line in f:
        if line != '\n':
            try:
                vector = create_vector_from_line(line)
                if len(vector) != len(centroids[0]):
                    raise
            except:
                raise Exception("Invalid Input!")


    f.close()
    return centroids
    

def load_args():
    inp = sys.argv
    args = {}
    if len(inp) < 4:
        raise Exception("Invalid Input!")
    try:
        args[Env.k] = int(inp[1])
        if args[Env.k] < 1:
            raise

        args[Env.output_file] = inp[-1]
        args[Env.input_file] = inp[-2]

        if len(inp) == 5: #Checking maxiter exists
            maxiter = int(inp[2])
            if maxiter <= 0: #Validating maxiter value
                raise
            else:
                args[Env.maxiter] = maxiter #If exists, added to args.
        
    except Exception:
        raise Exception("Invalid Input!")
    
    return args


def get_closest(centroids: list[Centroid], x: list[float]) -> int:
    
    minimal_distance = float('inf')
    minimal_index = -1

    for i, c in enumerate(centroids):
        distance_i = c.distance(x)
        if distance_i < minimal_distance:
            minimal_distance = distance_i
            minimal_index = i
    
    return minimal_index

    
def kmeans_iteration(centroids: list[list[float]], input_name: str) -> list[list[float]]:
    new_centroids = [Centroid() for i in range(len(centroids))]
    
    try:
        f = open(input_name, mode="r")
    except:
        raise Exception("An Error Has Occurred")

    try:
        for line in f:   
            xi = create_vector_from_line(line)
            closest_index = get_closest(centroids, xi)
            new_centroids[closest_index].add_vector(xi)
    except:
            raise Exception("An Error Has Occurred")
    finally:
            f.close()

    return new_centroids


def write_output(output_name, centroids):
    try:
        f = open(output_name, mode='w')
    except Exception as ex:
        print(ex)
        raise Exception("An Error Has Occurred")
    
    try:
        for centroid in centroids:
            for i in range(len(centroid)):
                f.write('%.4f' % centroid[i])
                if i < len(centroid)-1:
                    f.write(",")
            f.write('\n')
    except Exception as ex:
        print(ex)
        raise Exception("An Error Has Occurred")
    finally:
        f.close()

def checkForZeros(centroids: list[Centroid]) -> bool:
    for c in centroids:
        if c.count == 0:
            return True
    return False



main()