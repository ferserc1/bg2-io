
#include "types.h"
#include "errors.h"

int bg2io_allocateFloatArray(Bg2ioFloatArray * array, int length)
{
    if (array == NULL)
    {
        return BG2IO_ERR_INVALID_PTR;
    }
    else if (length <= 0) {
        return BG2IO_ERR_INVALID_LENGTH;
    }

    array->data = malloc(sizeof(float) * length);
    array->length = length;

    return BG2IO_NO_ERROR;
}

int bg2io_freeFloatArray(Bg2ioFloatArray * array)
{
    if (array == NULL) {
        return BG2IO_ERR_INVALID_PTR;
    }

    if (array->data != NULL)
    {
        free(array->data);
        array->data = NULL;
        array->length = 0;
    }

    return BG2IO_NO_ERROR;
}

int bg2io_allocateIntArray(Bg2ioIntArray * array, int length)
{
    if (array == NULL)
    {
        return BG2IO_ERR_INVALID_PTR;
    }
    else if (length <= 0) {
        return BG2IO_ERR_INVALID_LENGTH;
    }

    array->data = malloc(sizeof(int) * length);
    array->length = length;

    return BG2IO_NO_ERROR;
}

int bg2io_freeIntArray(Bg2ioIntArray * array)
{
    if (array == NULL) {
        return BG2IO_ERR_INVALID_PTR;
    }

    if (array->data != NULL)
    {
        free(array->data);
        array->data = NULL;
        array->length = 0;
    }

    return BG2IO_NO_ERROR;
}

Bg2ioPolyList * bg2io_createPolyList()
{
    Bg2ioPolyList * result = malloc(sizeof(Bg2ioPolyList));
    BG2IO_POLY_LIST_PTR_INIT(result);
    return result;
}

int bg2io_freePolyList(Bg2ioPolyList * plist)
{
    if (plist == NULL)
    {
        return BG2IO_ERR_INVALID_PTR;
    }
    int error = BG2IO_NO_ERROR;

    if (plist->name != NULL)
    {
        free(plist->name);
        plist->name = NULL;
    }

    if (plist->groupName != NULL)
    {
        free(plist->groupName);
        plist->groupName = NULL;  
    }

    if (plist->matName != NULL)
    {
        free(plist->matName);
        plist->matName = NULL;
    }

    bg2io_freeFloatArray(&plist->vertex);
    bg2io_freeFloatArray(&plist->normal);
    bg2io_freeFloatArray(&plist->texCoord0);
    bg2io_freeFloatArray(&plist->texCoord1);
    bg2io_freeFloatArray(&plist->texCoord2);
    bg2io_freeIntArray(&plist->index);
    
    if (plist->componentData != NULL)
    {
        free(plist->componentData);
        plist->componentData = NULL;
    }

    if (plist->componentData != NULL)
    {
        free(plist->componentData);
        plist->componentData = NULL;
    }

    return error;
}

Bg2ioPolyListArray * bg2io_createPolyListArray(int length)
{
    Bg2ioPolyListArray * result = NULL;

    if (length > 0)
    {
        result = malloc(sizeof(Bg2ioPolyListArray));
        BG2IO_POLY_LIST_ARRAY_PTR_INIT(result);

        result->data = malloc(sizeof(Bg2ioPolyList) * length);
        for (int i = 0; i < length; ++i)
        {
            result->data[i] = bg2io_createPolyList();
        }
        result->length = length;
    }

    return result;
}

int bg2io_freePolyListArray(Bg2ioPolyListArray * arr)
{
    if (arr == NULL)
    {
        return BG2IO_ERR_INVALID_PTR;
    }

    for (int i = 0; i < arr->length; ++i)
    {
        bg2io_freePolyList(arr->data[i]);
    }
    free(arr->data);
    arr->data = NULL;
    arr->length = 0;

    return BG2IO_NO_ERROR;
}
