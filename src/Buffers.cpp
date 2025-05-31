#include "Buffers.h"

// Счетчики использований дескрипторов
std::map<GLuint, GLuint> VAO::handler_count;    
std::map<GLuint, GLuint> BO::handler_count; 

VAO::VAO()
{ 
    glGenVertexArrays(1, &handler);
    glBindVertexArray(handler);
    handler_count[handler] = 1;
}

VAO::~VAO()
{
    if (!--handler_count[handler])
    {
        glDeleteVertexArrays(1, &handler);
        handler_count.erase(handler);
    }
}

VAO::VAO(const VAO & copy) : handler(copy.handler)
{
    handler_count[handler]++;
}

VAO& VAO::operator=(const VAO & other) 
{
    if (this != &other && handler != other.handler)
    {
        this->~VAO();
        handler = other.handler;
        handler_count[handler]++;
    }
    return *this;
}

void VAO::use()
{
    glBindVertexArray(handler);
}

void VAO::disable()
{
    glBindVertexArray(0);
}

BO::BO(BUFFER_TYPE t) : type(t)
{
    glGenBuffers(1, &handler);
    handler_count[handler] = 1;
    use();
}

BO::BO(BUFFER_TYPE t, const void *data, int size) : BO(t)
{
    load(data, size);
}

BO::~BO()
{
    if (handler)
    {
        if (!--handler_count[handler])
        {
            glDeleteBuffers(1, &handler);
            handler_count.erase(handler);
        }
        handler = 0;
    }
}

BO::BO(const BO & copy) : handler(copy.handler), type(copy.type)
{
    handler_count[handler]++;
}

BO& BO::operator=(const BO & other)
{
    if (this != &other && handler != other.handler)
    {
        this->~BO();
        handler = other.handler;
        handler_count[handler]++;
    }
    type = other.type;
    return *this;
}

void BO::load(const void *data, int size, GLuint mode)
{
    if (!handler) return;
    
    use();
    GLenum bufferType = (type == VERTEX) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
    glBufferData(bufferType, size, data, mode);
}

void BO::use()
{
    if (!handler) return;
    
    GLenum bufferType = (type == VERTEX) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(bufferType, handler);
}