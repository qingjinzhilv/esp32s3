SET(SOURCES screens/ui_Screen3.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_exit_png.c
    images/ui_img_w_c_png.c)

add_library(ui ${SOURCES})
