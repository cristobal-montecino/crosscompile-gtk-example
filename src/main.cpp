#include <gtkmm.h>
#include <glibmm.h>

int main(int argc, char **argv) {
    const auto applicationId = "org.cristobalmontecino.gtkexample";
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(applicationId);

    Gtk::Window window {};
    
    const auto windowTitle = "Example";
    window.set_title(windowTitle);

    app->run(window);
    return 0;
}