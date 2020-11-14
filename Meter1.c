 /*  g++ or gcc -Wall -Wextra -o main main.cpp $(pkg-config gtk+-3.0 --cflags --libs) -lm -rdynamic
 */
 
 
 #include <gtk/gtk.h>
 #include <stdlib.h>
 #include <math.h>
 
 
 
 
 void quitButtonClicked(__attribute__((unused)) GtkWidget *widget, 
                        __attribute__((unused)) gpointer   data)
 {
     g_print("Quit Clicked\n");
     gtk_main_quit();
 }
 
 
 // Handle the user trying to close the window 
 gboolean windowDelete(__attribute__((unused)) GtkWidget *widget, 
                       __attribute__((unused)) GdkEvent  *event,
                       __attribute__((unused)) gpointer   data)
 {
     return TRUE;    // Returning TRUE stops the window being deleted.
                     // Returning FALSE allows deletion.   
 }
 
 // This just causes the drawing area to be redrawn when the slider is moved.
 // user_data is a pointer to drawingarea1
 void on_scale1_value_changed(__attribute__((unused)) GtkRange *range,
                              gpointer  user_data)
 {
     
     gtk_widget_queue_draw(GTK_WIDGET(user_data));
 }
 
 
 // user_data is a pointer to slider1
 gboolean on_drawingarea1_draw(__attribute__((unused)) GtkWidget *widget,
                                      cairo_t *cr, 
                                      gpointer user_data)
 {
     gdouble marks;
     char text[10];
     
     static PangoLayout *layout = NULL;  // Retained between calls
     PangoFontDescription *desc;
     int width,height;
 
     gdouble value;
 
     // Do some pango setup on the first call.
     if(layout == NULL)
     {
         layout = pango_cairo_create_layout (cr);
         desc = pango_font_description_from_string ("Sans Bold 15");
         pango_layout_set_font_description (layout, desc);
         pango_font_description_free (desc);
     }
 
     cairo_set_source_rgb(cr, 0, 0, 0);
     cairo_set_line_width(cr, 1.0);
 
     // Set origin as middle of bottom edge of the drawing area.
     // Window is not resizable so "magic numbers" will work here. 
     cairo_translate(cr,200,190);
 
 
     // Draw the black radial scale marks and labels
     for(marks = 0.0; marks <= 100.0; marks += 10.0)
     {
         // Draw the radial marks
         cairo_move_to(cr, 150.0 * cos(M_PI * marks * 0.01),   -150.0 * sin(M_PI * marks * 0.01) );
         cairo_line_to(cr, 160.0 * cos(M_PI * marks * 0.01),   -160.0 * sin(M_PI * marks * 0.01) );
 
         // Set the text to print
         sprintf(text,"%2.0f", marks);
         pango_layout_set_text (layout, text, -1);
 
         // Redo layout for new text
         pango_cairo_update_layout (cr, layout);
 
         // Retrieve the size of the rendered text
         pango_layout_get_size (layout, &width, &height);
 
         // Position the text at the end of the radial marks
         cairo_move_to(cr, 
                       (-180.0 * cos(M_PI *marks * 0.01)) - ((double)width /PANGO_SCALE / 2.0),
                       (-180.0 * sin(M_PI *marks * 0.01)) - ((double)height/PANGO_SCALE / 2.0));
         // Render the text
         pango_cairo_show_layout (cr, layout);
     }
     cairo_stroke(cr);
 
     // Retrieve the new slider value
     value = gtk_range_get_value(GTK_RANGE(user_data));
 
     cairo_set_source_rgb(cr, 1.0, 0, 0);
     cairo_set_line_width(cr, 1.5);
 
     // Draw the meter pointer
     cairo_move_to(cr, 0.0,0.0);
     cairo_line_to(cr, -150.0 * cos(M_PI *value * 0.01),  -150.0 * sin(M_PI *value * 0.01) );
     cairo_stroke(cr);  
 
     return FALSE;
 }
 
 
 // main is a generic version for a glade project.
 
int main(int argc, char **argv) 
{
    GtkBuilder *builder = NULL;
 
    gtk_init (&argc , &argv);
 
    builder = gtk_builder_new();
 
    if( gtk_builder_add_from_file (builder,"Meter1.glade" , NULL) == 0)
    {
         printf("gtk_builder_add_from_file FAILED\n");
         return EXIT_FAILURE;
    }
 
    gtk_builder_connect_signals(builder,NULL);

    gtk_main ();
    return EXIT_SUCCESS;
}
