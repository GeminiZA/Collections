#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <string>
#include <sstream>
#include <stdexcept>

namespace Collections
{

    template <typename T>
    class List;

    template <typename T>
    class Vector;

    template <typename T>
    class HashSet;

    template <typename T>
    class Collection
    {
    public:
        Collection(){};
        virtual ~Collection(){};
        virtual List<T> toList() const = 0;
        virtual Vector<T> toVec() const = 0;
        // virtual HashSet<T> toHashSet() const = 0;
        virtual std::string toString() const = 0;
    };

    template <typename T>
    class Vector : public Collection<T>
    {
    private:
        T *data;
        size_t capacity;
        size_t count;
        void _resize()
        {
            if (count == capacity)
            {
                T *temp = new T[capacity * 2];
                for (size_t i = 0; i < count; i++)
                    temp[i] = data[i];
                delete[] data;
                data = temp;
            }
        }

    public:
        Vector() : data(new T[1]), capacity(1), count(0) {}
        Vector(size_t size)
        {
            data = new T[size];
            capacity = size;
            count = 0;
        }
        ~Vector()
        {
            delete[] data;
        }
        void push_back(const T &val)
        {
            _resize();
            data[count] = val;
            count++;
        }
        void push_front(const T &val)
        {
            if (count == capacity)
            {
                T *temp = new T[capacity * 2];
                for (size_t i = 0; i < count; i++)
                    temp[i + 1] = data[i];
                delete[] data;
                data = temp;
            }
            else
                for (size_t i = 0; i < count; i++)
                    data[i + 1] = data[i];
            data[0] = val;
        }
        void insert(const T &val, const size_t &index)
        {
            if (index >= count)
                throw std::out_of_range("Index out of bounds");
            _resize();
            data[index] = val;
            count++;
        }
        T get(const size_t &index)
        {
            if (index >= count)
                throw std::out_of_range("Index out of bounds");
            return data[index];
        }
        void set(const T &val, const size_t &index)
        {
            if (index >= count)
                throw std::out_of_range("Index out of bounds");
            data[index] = val;
        }
        T &operator[](size_t index)
        {
            if (index >= count)
                throw std::out_of_range("Index out of bounds");
            return data[index];
        }

        std::string toString() const override
        {
            if (count == 0)
                return "<>";
            std::stringstream ss = std::stringstream();
            ss << '<';
            for (size_t i = 0; i < count - 1; i++)
                ss << data[i] << ", ";
            ss << data[count - 1] << '>';
            return ss.str();
        }

        List<T> toList() const override
        {
            List<T> l = List<T>();
            for (size_t i = 0; i < count; i++)
                l.push_back(data[i]);
            return l;
        }
        Vector<T> toVec() const override
        {
            return *this;
        }
        Vector<T> map(T (*operation)(T))
        {
            Vector<T> temp = Vector<T>();
            for (size_t i = 0; i < count; i++)
                temp.push_back(operation(data[i]));
            return temp;
        }
    };

    template <typename T>
    struct Node
    {
        T data;
        Node *next;
        Node *prev;

        Node(const T &value) : data(value), next(nullptr), prev(nullptr) {}
    };

    template <typename T>
    class List : public Collection<T>
    {
    private:
        size_t count;
        Node<T> *first;
        Node<T> *last;
        Node<T> *_getNode(size_t index)
        {
            if (index >= count)
                throw std::out_of_range("Index out of bounds");
            Node<T> *cur;
            size_t cur_index;
            if (index >= count / 2)
            {
                cur = last;
                cur_index = count - 1;
                while (cur_index-- != index)
                    cur = cur->prev;
                return cur;
            }
            else
            {
                cur = first;
                cur_index = 0;
                while (cur_index++ != index)
                    cur = cur->next;
                return cur;
            }
        }

    public:
        List() : first(nullptr), last(nullptr), count(0) {}
        List(const T &val)
        {
            Node<T> *newNode = new Node<T>(val);
            this->first = newNode;
            this->last = newNode;
            this->count = 1;
        }
        size_t size()
        {
            return count;
        }
        void push_back(const T &val)
        {
            Node<T> *newNode = new Node<T>(val);
            if (count > 0)
            {
                newNode->prev = last;
                last->next = newNode;
            }
            else
                first = newNode;
            last = newNode;
            count++;
        }
        void push_front(const T &val)
        {
            Node<T> *newNode = new Node<T>(val);
            if (count > 0)
            {
                newNode->next = first;
                first->prev = newNode;
            }
            else
                last = newNode;
            first = newNode;
            count++;
        }
        T pop_back()
        {
            if (count < 1)
            {
                throw std::out_of_range("Index out of bounds");
            }
            if (count < 2)
            {
                count--;
                T temp = first->data;
                first = nullptr;
                last = nullptr;
                count--;
                return temp;
            }
            else
            {
                count--;
                Node<T> *temp = last->prev;
                T val = last->data;
                temp->next = nullptr;
                last = temp;
                return val;
            }
        }
        T pop_front()
        {
            if (count < 1)
            {
                throw std::out_of_range("Index out of bounds");
            }
            if (count < 2)
            {
                count--;
                T temp = first->data;
                first = nullptr;
                last = nullptr;
                return temp;
            }
            else
            {
                count--;
                Node<T> *temp = first->next;
                T val = first->data;
                temp->prev = nullptr;
                first = temp;
                return val;
            }
        }
        void set(const size_t index, const T &val)
        {
            if (index >= count)
                throw std::out_of_range("Index out of bounds");
            Node<T> *cur = this->_getNode(index);
            cur->data = val;
        }

        T get(const size_t index)
        {
            if (index >= count)
                throw std::out_of_range("Index out of bounds");
            Node<T> *cur = this->_getNode(index);
            return cur->data;
        }
        size_t find(const T &val)
        {
            Node<T> *cur = first;
            size_t i = 0;
            while (i < count)
            {
                if (cur->data == val)
                    return i;
                i++;
                cur = cur->next;
            }
            return -1;
        }

        T &operator[](size_t index)
        {
            if (index >= count)
                throw std::out_of_range("index out of bounds");
            else
            {
                Node<T> *cur = this->_getNode(index);
                return cur->data;
            }
        }

        std::string toString() const override
        {
            if (this->count == 0)
                return "[]";
            std::stringstream ss = std::stringstream();
            Node<T> *cur = this->first;
            ss << "[";
            while (cur != this->last)
            {
                ss << cur->data << ", ";
                cur = cur->next;
            }
            ss << cur->data << "]";
            return ss.str();
        }

        List<T> toList() const override
        {
            return *this;
        }

        Vector<T> toVec() const override
        {
            Vector<T> v = Vector<T>();
            Node<T> *cur = first;
            while (cur != nullptr)
            {
                v.push_back(cur->data);
            }
            return v;
        }

        List<T> map(T (*operation)(T))
        {
            List<T> temp = List<T>();
            if (count == 0)
                return temp;
            Node<T> *cur = first;
            while (cur != last)
            {
                temp.push_back(operation(cur->data));
                cur = cur->next;
            }
            temp.push_back(operation(cur->data));
            return temp;
        }
    };

}
#endif