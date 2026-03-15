void list_repack(struct ListStruct* list) {
    if (list == NULL || list->head == NULL)
        return;

    struct ListNode* current = list->head;
    while (current != NULL && current->next != NULL) {
        while (current->count < BLOCK_SIZE && current->next != NULL) {
            current->data[current->count] = current->next->data[0];
            current->count++;

            for (int i = 0; i < current->next->count - 1; i++)
                current->next->data[i] = current->next->data[i + 1];
            current->next->count--;

            if (current->next->count == 0) {
                struct ListNode* node_to_delete = current->next;
                current->next = current->next->next;
                free(node_to_delete);
            }
        }
        current = current->next;
    }
}

struct ListStruct* list_init() {
    struct ListStruct* list = malloc(sizeof(struct ListStruct));

    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;

    return list;
}

void list_destroy(struct ListStruct* list) {
    if (list == NULL) return;

    struct ListNode* curr = list->head;

    while (curr != NULL) {
        struct ListNode* temp = curr->next;

        free(curr);

        curr = temp;
    }

    free(list);
}

bool list_is_empty(struct ListStruct* list) {
    if (list == NULL) return true;
    return list->head == NULL;
}

int list_count(struct ListStruct* list) {
    if (list == NULL || list->head == NULL) return 0;
    int counter = 0;

    struct ListNode* curr = list->head;

    while (curr != NULL) {
        counter += curr->count;
        curr = curr->next;
    }

    return counter;
}

struct ListNode* list_get(struct ListStruct* list, int index, int* index_in_block) {
    if (list == NULL || list->head == NULL) return NULL;

    struct ListNode* curr = list->head;
    int counter = list_count(list);

    if (index >= counter || index < 0) return NULL;

    while (curr != NULL) {
        if (index < curr->count) {
            *index_in_block = index;
            return curr;
        }

        index -= curr->count;
        curr = curr->next;
    }

    return NULL;
}

void list_push_front(struct ListStruct* list, int data) {
    if (list == NULL) return;

    if (list->head == NULL) {
        struct ListNode* node = malloc(sizeof(struct ListNode));
        if (!node) return;
        node->count = 1;
        node->data[0] = data;
        node->next = NULL;
        list->head = node;
        return;
    }

    struct ListNode* curr = list->head;

    while (curr != NULL) {
        if (curr->count < BLOCK_SIZE) {
            for (int i = curr->count; i > 0; i--) {
                curr->data[i] = curr->data[i - 1];
            }
            curr->data[0] = data;
            curr->count++;
            return;
        }

        int displaced_data = curr->data[BLOCK_SIZE - 1];
        for (int i = BLOCK_SIZE - 1; i > 0; i--) {
            curr->data[i] = curr->data[i - 1];
        }
        curr->data[0] = data;

        data = displaced_data;

        if (curr->next == NULL) {
            struct ListNode* new_node = malloc(sizeof(struct ListNode));
            if (!new_node) return;
            new_node->count = 1;
            new_node->data[0] = displaced_data;
            new_node->next = NULL;
            curr->next = new_node;
            return;
        }

        curr = curr->next;
    }
    list_repack(list);
}

void list_push_back(struct ListStruct* list, int data) {
    if (list == NULL) return;

    if (list->head == NULL) {
        struct ListNode* node = malloc(sizeof(struct ListNode));
        if (!node) return;
        node->count = 1;
        node->data[0] = data;
        node->next = NULL;
        list->head = node;
        list_repack(list);
        return;
    }
    list_repack(list);

    struct ListNode* curr = list->head;

    while (curr->next != NULL) {
        curr = curr->next;
    }

    if (curr->count < BLOCK_SIZE) {
        curr->data[curr->count] = data;
        curr->count++;
        list_repack(list);
        return;
    }

    struct ListNode* new_node = malloc(sizeof(struct ListNode));
    if (!new_node) return;
    new_node->count = 1;
    new_node->data[0] = data;
    new_node->next = NULL;
    curr->next = new_node;
    list_repack(list);
}

void list_push(struct ListStruct* list, int index, int data) {
    if (list == NULL) return;

    if (list->head == NULL) {
        struct ListNode* node = malloc(sizeof(struct ListNode));
        if (!node) return;
        node->count = 1;
        node->data[0] = data;
        node->next = NULL;
        list->head = node;
        list_repack(list);
        return;
    }

    list_repack(list);
    if (index < 0) {
        list_push_front(list, data);
        list_repack(list);
        return;
    }
    if (index >= list_count(list)) {
        list_push_back(list, data);
        list_repack(list);
        return;
    }
    int index_in_block;
    struct ListNode* curr = list_get(list, index, &index_in_block);
    if (curr == NULL) return;

    while (curr != NULL) {
        if (curr->count < BLOCK_SIZE) {
            for (int i = curr->count; i > index_in_block; i--) {
                curr->data[i] = curr->data[i - 1];
            }
            curr->data[index_in_block] = data;
            curr->count++;
            list_repack(list);
            return;
        }

        int displaced_data = curr->data[BLOCK_SIZE - 1];
        for (int i = BLOCK_SIZE - 1; i > index_in_block; i--) {
            curr->data[i] = curr->data[i - 1];
        }
        curr->data[index_in_block] = data;

        data = displaced_data;
        index_in_block = 0;

        if (curr->next == NULL) {
            struct ListNode* new_node = malloc(sizeof(struct ListNode));
            if (!new_node) return;
            new_node->count = 1;
            new_node->data[0] = data;
            new_node->next = NULL;
            curr->next = new_node;
            list_repack(list);
            return;
        }

        curr = curr->next;
    }
}

void list_pop(struct ListStruct* list, int index) {
    if (list->head == NULL || index < 0 || index >= list_count(list)) return;
    list_repack(list);
    int index_in_block;
    struct ListNode* curr = list_get(list, index, &index_in_block);
    if (curr == NULL) return;

    while (curr != NULL) {
        for (int i = index_in_block; i < curr->count - 1; i++) {
            curr->data[i] = curr->data[i + 1];
        }

        if (curr->next != NULL) {
            int next_node_element = curr->next->data[0];
            curr->data[curr->count - 1] = next_node_element;

            curr = curr->next;
            index_in_block = 0;
        } else {
            curr->count--;

            if (curr->count == 0) {
                if (list->head == curr) {
                    list->head = NULL;
                    free(curr);
                } else {
                    struct ListNode* prev = list->head;
                    while (prev != NULL && prev->next != curr) {
                        prev = prev->next;
                    }
                    if (prev != NULL) {
                        prev->next = curr->next; /* == NULL */
                    }
                    free(curr);
                }
            }
            list_repack(list);
            return;
        }
    }
    list_repack(list);
}

int cmp(const void* a, const void* b) {
    return *(const int*) a - *(const int*) b;
}

void list_sort(struct ListStruct* list) {
    if (list->head == NULL) return;
    list_repack(list);

    struct ListNode* head = list->head;
    struct ListNode* curr = head;


    int list_data_size = list_count(list);
    int* sorted_data = malloc(list_data_size * sizeof(int));
    int global_idx = 0;

    while (curr != NULL) {
        for (int i = 0; i < curr->count; i++) {
            sorted_data[global_idx] = curr->data[i];
            global_idx++;
        }
        curr = curr->next;
    }

    qsort(sorted_data, list_data_size, sizeof(int), cmp);
    curr = head;
    global_idx = 0;

    while (curr != NULL) {
        for (int i = 0; i < curr->count; i++) {
            curr->data[i] = sorted_data[global_idx];
            global_idx++;
        }
        curr = curr->next;
    }
    list_repack(list);
    free(sorted_data);
}

void list_remove_duplicates(struct ListStruct* list) {
    if (list == NULL || list->head == NULL) return;
    list_repack(list);

    for (int i = 0; i < list_count(list); i++) {
        int index_in_block_i;
        struct ListNode* node_i = list_get(list, i, &index_in_block_i);
        if (node_i == NULL) continue; /* защита */
        int target = node_i->data[index_in_block_i];

        for (int j = i + 1; j < list_count(list);) {
            int index_in_block_j;
            struct ListNode* node_j = list_get(list, j, &index_in_block_j);
            if (node_j == NULL) break;
            if (target == node_j->data[index_in_block_j]) {
                list_pop(list, j);
            } else {
                j++;
                // Не инкрементируем итератор j в цикле for при удалении, так как данные "приезжают" на текущий индекс сами.
            }
        }
    }
    list_repack(list);
}

double list_median(struct ListStruct* list) {
    if (list->head == NULL || list_count(list) == 0) return 0;

    struct ListNode* head = list->head;
    struct ListNode* curr = head;

    int list_data_size = list_count(list);
    int* data_list = malloc(list_data_size * sizeof(int));
    int global_idx = 0;
    while (curr != NULL) {
        for (int i = 0; i < curr->count; i++) {
            data_list[global_idx] = curr->data[i];
            global_idx++;
        }
        curr = curr->next;
    }

    qsort(data_list, list_data_size, sizeof(int), cmp);

    if (list_data_size % 2 == 0) {
        int left_mid = data_list[list_data_size / 2 - 1];
        int right_mid = data_list[list_data_size / 2];

        double result = (left_mid + right_mid) / 2.0;
        free(data_list);
        return result;
    }
    double result = data_list[list_data_size / 2];
    free(data_list);
    return result;
}

void list_reverse_blocks(struct ListStruct* list) {
    if (list == NULL || list->head == NULL || list->head->next == NULL) return;
    struct ListNode* prev = NULL;
    struct ListNode* curr = list->head;
    struct ListNode* next = NULL;

    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    list->head = prev;
}
