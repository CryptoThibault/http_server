NAME = httpServer
SRCS = main.cpp FileDescriptor.cpp Listener.cpp Connection.cpp \
	ThreadSafeQueue.cpp ThreadPool.cpp
OBJS = $(SRCS:.cpp=.o)
CXX = c++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re