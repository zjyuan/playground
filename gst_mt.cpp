#include <boost/thread.hpp>
#include <iostream>

#include <gst/gst.h>
 
#include <string.h>
#include <time.h> 
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>

using namespace std;

//using gstreamer in multi-thread context

typedef struct
{
  GMainLoop *loop;
  GstElement *source;
  GstElement *sink;
  const char* rtsp_url;
} ProgramData;
 
/* user modify the data here */
 
static GstFlowReturn
modify_in_data (GstMapInfo * map)
{
  int dataLength;
  guint8 *rdata;
    
  dataLength = map->size;
  rdata = map->data;
  g_print ("%s dataLen = %d\n", __func__, dataLength);
   
  /* Modify half of frame to plane white */
  for (int i=0; i <= dataLength/2; i++) {
      rdata[i] = 0xff;
  }
}
 
/* called when the appsink notifies us that there is a new buffer ready for
 * processing */
static GstFlowReturn
on_new_frame_sample (GstElement * elt, ProgramData * data)
{
  GstSample *sample;
  GstBuffer *app_buffer, *buffer;
  GstElement *source;
  GstFlowReturn ret;
  GstMapInfo map;
  guint8 *rdata;
  int dataLength;
  static int i=0;
  i++;
  g_print ("%s %s %d\n", __func__, data->rtsp_url, i);

 
  return GST_FLOW_OK;
}

static GstFlowReturn on_new_audio_sample(GstElement * elt, ProgramData * data)
{
  static int i=0;
  i++;
  time_t ts = time(0);
  g_print ("%s %d %d\n", __func__, i, ts);
  return GST_FLOW_OK;
}
 
/* called when we get a GstMessage from the source pipeline when we get EOS, we
 * notify the appsrc of it. */
static gboolean
on_source_message (GstBus * bus, GstMessage * message, ProgramData * data)
{
  GstElement *source;
  // g_print ("%s\n", __func__);
 
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_EOS:
      g_print ("The source got dry\n");
      source = gst_bin_get_by_name (GST_BIN (data->sink), "testsource");
      gst_app_src_end_of_stream (GST_APP_SRC (source));
      gst_object_unref (source);
      break;
    case GST_MESSAGE_ERROR:
      g_print ("Received error\n");
      g_main_loop_quit (data->loop);
      break;
    default:
      break;
  }
  return TRUE;
}
 
/* called when we get a GstMessage from the sink pipeline when we get EOS, we
 * exit the mainloop and this testapp. */
static gboolean
on_sink_message (GstBus * bus, GstMessage * message, ProgramData * data)
{
  /* nil */
  g_print ("%s\n", __func__);
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_EOS:
      g_print ("Finished playback\n");
      g_main_loop_quit (data->loop);
      break;
    case GST_MESSAGE_ERROR:
      g_print ("Received error\n");
      g_main_loop_quit (data->loop);
      break;
    default:
      break;
  }
  return TRUE;
}
 
int
process_rtsp_stream (const char* rtsp_url)
{
  ProgramData *data = NULL;
  gchar *string = NULL;
  GstBus *bus = NULL;
  GstElement *testsink = NULL;
  GstElement *testsource = NULL;
 
  gst_init (NULL, NULL);
 
  data = g_new0 (ProgramData, 1);
 
  data->loop = g_main_loop_new (NULL, FALSE);

  data->rtsp_url = rtsp_url;
 
  /* setting up source pipeline, we read from a file and convert to our desired
   * caps. */
  // string =
  //     g_strdup_printf
  //     ("rtspsrc location=%s name=d d. ! \
  //       queue ! capsfilter caps=application/x-rtp,media=video ! decodebin3 ! nvvidconv ! videorate ! video/x-raw,framerate=1/1 ! nvjpegenc ! appsink name=video_sink d. ! \
  //       queue ! capsfilter caps=application/x-rtp,media=audio ! decodebin ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,rate=16000,channels=1 ! audiobuffersplit output-buffer-duration=1/1 ! appsink name=audio_sink", argv[1]);
  

  string =
      g_strdup_printf
      ("rtspsrc location=%s name=d d. ! \
        queue ! capsfilter caps=application/x-rtp,media=video ! decodebin3 ! nvvidconv ! videorate ! video/x-raw,framerate=1/1 ! nvjpegenc ! appsink name=video_sink", rtsp_url);
  
      // string =
      // g_strdup_printf
      // ("rtspsrc location=rtsp://192.168.28.150:8554/1 ! queue ! capsfilter caps=application/x-rtp,media=audio ! decodebin ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,rate=16000,channels=1 ! audiobuffersplit output-buffer-duration=1/1 ! appsink name=audio_sink");
  
  data->source = gst_parse_launch (string, NULL);
  g_free (string);
 
  if (data->source == NULL) {
    g_print ("Bad source\n");
    g_main_loop_unref (data->loop);
    g_free (data);
    return -1;
  }
 
  g_print ("Capture bin launched\n");
  /* to be notified of messages from this pipeline, mostly EOS */
  bus = gst_element_get_bus (data->source);
  gst_bus_add_watch (bus, (GstBusFunc) on_source_message, data);
  gst_object_unref (bus);
 
  /* we use appsink in push mode, it sends us a signal when data is available
   * and we pull out the data in the signal callback. */
  testsink = gst_bin_get_by_name (GST_BIN (data->source), "video_sink");
  g_object_set (G_OBJECT (testsink), "emit-signals", TRUE, "sync", TRUE, NULL);
  g_signal_connect (testsink, "new-sample",
      G_CALLBACK (on_new_frame_sample), data);
  gst_object_unref (testsink);

  // GstElement *audio_sink = gst_bin_get_by_name(GST_BIN(data->source), "audio_sink");
  // g_object_set (G_OBJECT (audio_sink), "emit-signals", TRUE, "sync", TRUE, NULL);
  // g_signal_connect (audio_sink, "new-sample",
  //     G_CALLBACK (on_new_audio_sample), data);
  // gst_object_unref (audio_sink);
 
  g_print ("Going to set state to play\n");
  /* launching things */
  // gst_element_set_state (data->sink, GST_STATE_PLAYING);
  gst_element_set_state (data->source, GST_STATE_PLAYING);
 
  /* let's run !, this loop will quit when the sink pipeline goes EOS or when an
   * error occurs in the source or sink pipelines. */
  g_print ("Let's run!\n");
  g_main_loop_run (data->loop);
  g_print ("Going out\n");
 
  gst_element_set_state (data->source, GST_STATE_NULL);
  gst_element_set_state (data->sink, GST_STATE_NULL);
 
  gst_object_unref (data->source);
  gst_object_unref (data->sink);
  g_main_loop_unref (data->loop);
  g_free (data);
 
  return 0;
}

void launch_gst_pipeline(const char* rtsp_url)
{
	cout << "thread: processing " << rtsp_url << endl;
	process_rtsp_stream(rtsp_url);
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		cout << "wrong number of argumens" << endl;
		return -1;
	}

	boost::thread_group threads;
	threads.create_thread(bind(launch_gst_pipeline, argv[1]));
	threads.create_thread(bind(launch_gst_pipeline, argv[2]));
	threads.join_all();
}

//g++ gst_mt.cpp -o gst_mt -std=c++11 -lpthread -lboost_thread `pkg-config --cflags --libs gstreamer-1.0` -lgstapp-1.0