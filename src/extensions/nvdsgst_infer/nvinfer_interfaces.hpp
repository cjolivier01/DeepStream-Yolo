#pragma once

#include <glib.h>
#include <gst/gst.h>

#include "nvdsinterface/interfaces.hpp"

namespace nvidia {
namespace deepstream {

/**
 * @brief model-updated placeholder description. This is a connector component.
 * This component must be linked to 'GstNvInfer' which emits the signal and
 * another component which will handle the signal callback.
 */
struct NvDsModelUpdatedSignal : public INvDsSignal {
  /**
   * @brief Signal callback handler prototype
   */
  struct Handler {
    virtual void on_model_updated(int placeholderArgName0,
                                  gchar *placeholderArgName1) = 0;
  };

  /**
   * @brief Set the signal handler
   */
  void set_handler(Handler *handler) {
    this->handler = handler;
    install_signal_handler();
  }

 private:
  void install_signal_handler() override {
    if (element && element->get_element_ptr() && handler) {
      g_signal_connect(element->get_element_ptr(), "model-updated",
                       G_CALLBACK(signal_cb), this);
    }
  }

  static void signal_cb(GstElement *element, int placeholderArgName0,
                        gchar *placeholderArgName1, gpointer data) {
    return reinterpret_cast<NvDsModelUpdatedSignal *>(data)
        ->handler->on_model_updated(placeholderArgName0, placeholderArgName1);
  }

  Handler *handler;
};

}  // namespace deepstream
}  // namespace nvidia
