#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

struct bohr_vertex
{
    char *next, *transitions;
    int pattern_num, pattern_size;
    int state, suffix_link, parent;
    bool end;
    char pchar;

    bohr_vertex(int vocab_size, int parent, int state, char pchar)
    {
        end = false;
        this->parent = parent;
        next = new char[vocab_size];
        transitions = new char[vocab_size];
        suffix_link = -1;
        pattern_num = -1;
        pattern_size = -1;
        this->state = state;
        this->pchar = pchar;
        std::fill_n(next, vocab_size, -1);
        std::fill_n(transitions, vocab_size, -1);
    }
};

struct bohr
{
    int vocab_size = 128;
    std::vector<bohr_vertex> vertices;
    std::vector<std::string> patterns;
    bohr()
    {
        add_vertex(-1, '.');
    }

    void add_vertex(int parent, char pchar)
    {
        bohr_vertex vertex = bohr_vertex(vocab_size, parent, vertices.size(), pchar);
        vertices.push_back(vertex);
    }

    void add_string(std::string &s)
    {
        int state = 0;
        for (int i = 0; i != s.size(); i++)
        {
            if (vertices[state].next[s[i]] == -1)
            {
                add_vertex(vertices[state].state, s[i]);
                vertices[state].next[s[i]] = vertices.size() - 1;
            }
            state = vertices[state].next[s[i]];
        }
        vertices[state].end = true;
        vertices[state].pattern_num = patterns.size();
        vertices[state].pattern_size = s.size();
        patterns.push_back(s);
    }

    int get_link(int state)
    {
        bohr_vertex &vertex = vertices[state];
        if (vertex.suffix_link == -1)
        {
            if (vertex.state == 0 || vertices[vertex.parent].state == 0)
            {
                vertex.suffix_link = 0;
            }
            else
            {
                vertex.suffix_link = get_transition(get_link(vertex.parent), vertex.pchar);
            }
        }
        return vertex.suffix_link;
    }

    int get_transition(int state, char pchar)
    {
        bohr_vertex &vertex = vertices[state];
        if (vertex.transitions[pchar] == -1)
        {
            if (vertex.next[pchar] != -1)
            {
                vertex.transitions[pchar] = vertex.next[pchar];
            }
            else if (vertex.state == 0)
            {
                vertex.transitions[pchar] = 0;
            }
            else
            {
                vertex.transitions[pchar] = get_transition(get_link(state), pchar);
            }
        }
        return vertex.transitions[pchar];
    }

    std::vector<std::pair<int, int>> find_all_pos(std::string &text)
    {
        std::vector<std::pair<int, int>> answer_vector;
        int cur_state = 0;
        bool digit_flag = false;
        int start_digit;
        int size_digit = 0;
        for (int i = 0; i != text.size(); i++)
        {
            if (!digit_flag && (isdigit(text[i]) ||
                                (i + 1 < text.size() && text[i] == '-' && isdigit(text[i + 1]))))
            {
                digit_flag = true;
                start_digit = i;
                size_digit = 1;
                continue;
            }
            else if (digit_flag && (isdigit(text[i]) ||
                                    (i > 0 && i + 1 < text.size() && text[i] == '.' && text[i - 1] != '.' && isdigit(text[i + 1]))))
            {
                size_digit++;
                continue;
            }
            else if (digit_flag && !isdigit(text[i]))
            {
                answer_vector.push_back({start_digit, size_digit});
                digit_flag = false;
            }
            cur_state = get_transition(cur_state, text[i]);
            int state = cur_state;
            while (state != 0)
            {
                if (vertices[state].end)
                {
                    int p_num = vertices[state].pattern_num;
                    int p_size = vertices[state].pattern_size;
                    int index = i - p_size + 1;
                    answer_vector.push_back({index, p_size});
                }
                state = get_link(state);
            }
        }
        if (digit_flag)
        {
            answer_vector.push_back({start_digit, size_digit});
        }
        return answer_vector;
    }
};
