// contains all of the cases for changing from normal mode to different kinds of visual modes
#define VISUAL_TRANS_CASES() \
    case 'v': { \
        cursor->select_pos_x = cursor->x; \
        cursor->select_pos_y = cursor->y; \
        cursor->visual_state = ( cursor->visual_state == visual_state_NONE); \
        if (cursor->previous_char == 'r') { cursor->visual_state = visual_state_COLUMN_SELECT; } \
        client->redraw_spreadsheet = true; \
    } break; \
    case 'V': { \
        cursor->select_pos_y = cursor->y; \
        cursor->visual_state = ( cursor->visual_state == visual_state_NONE) * visual_state_ROW_SELECT; \
        client->redraw_spreadsheet = true; \
    } break;

