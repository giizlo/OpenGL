#ifndef BUFFERS_H
#define BUFFERS_H

#include <glad/glad.h>

#include <map>

// Объект массива вершин
class VAO
{
    public:
        VAO(); // Создает VAO и активирует его
        ~VAO(); // Уничтожает VAO
        VAO(const VAO & copy); // Конструктор копирования
        VAO& operator=(const VAO & other); // Оператор присваивания

        void use(); // Активация VAO
        static void disable(); // Деактивация активного VAO

    private:
        GLuint handler; // Дескриптор
        static std::map<GLuint, GLuint> handler_count; // Счетчик использований дескриптора
};

// Тип буфера
enum BUFFER_TYPE {  VERTEX = GL_ARRAY_BUFFER
                  , ELEMENT = GL_ELEMENT_ARRAY_BUFFER
                 };

// Объект вершинного буфера
class BO
{
    public:
        BO(BUFFER_TYPE type); // Создает пустой буфер заданного типа
        BO(BUFFER_TYPE type, const void *data, int size); // Создает и загружает туда данные
        ~BO(); // Уничтожает буфер
        BO(const BO & copy); // Конструктор копирования
        BO& operator=(const BO & other); // Оператор присваивания

        void load(const void *data, int size, GLuint mode = GL_STATIC_DRAW); // Загрузка данных в буфер
        void use(); 

    protected:
        GLuint handler; // Дескриптор
        BUFFER_TYPE type; // Тип буфера
    private:
        static std::map<GLuint, GLuint> handler_count; // Счетчик использований дескриптора
};

#endif // BUFFERS_H