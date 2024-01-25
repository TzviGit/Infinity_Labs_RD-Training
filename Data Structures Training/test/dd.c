


size_t AVLSize(const avl_t *avl)
{
    return AVLSubtreeHeight(avl->root);
}
size_t AVLSubtreeHeight(avl_node_t *node)
{
    if (NULL == node)
    {
        return 0;
    }

    return 1 + MAX(AVLSubtreeHeight(node->children[LEFT]),
                    AVLSubtreeHeight(node->children[LEFT]));
}
