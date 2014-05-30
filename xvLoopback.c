/* A program to do a loopback of video from capture to render.
   This program captures the video from first camera attached to the machine: '/dev/video0'
   and displays it on the screen for 10 seconds.

   It is not configurable from outside, but one can play with the resolution and frames per seconds
   by changing the macros. 

   Two capture formats are supported by this program YUV422 (YUYV) and YUV420P

   Author: Tanmay Mande

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include "libv4l2.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#include "libswscale/swscale.h"
#include "libavutil/pixfmt.h"
#include <time.h>

#define CAP_WIDTH 1280
#define CAP_HEIGHT 720

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define RENDER_WIDTH 1280
#define RENDER_HEIGHT 720

#define FPS 30

//#define FMT V4L2_PIX_FMT_YUYV
//#define SWS_FMT PIX_FMT_YUYV422

#define FMT V4L2_PIX_FMT_YUV420
#define SWS_FMT PIX_FMT_YUV420P

//#define FMT V4L2_PIX_FMT_RGB32

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define GUID_YUV12_PLANAR 0x32315659 /* TO DO */
#define GUID_UYVY 1498831189
#define GUID_I420 808596553
/* C A P T U R E */
struct buffer {
    void   *start;
    size_t length;
};

static void xioctl(int fh, int request, void *arg)
{
    int r;

    do {
        r = v4l2_ioctl(fh, request, arg);
    } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

    if (r == -1) {
        fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

char                            *dev_name = "/dev/video1";
struct v4l2_format              fmt;
struct v4l2_buffer              buf;
struct v4l2_requestbuffers      req;
enum v4l2_buf_type              type;
unsigned int                    i, n_buffers;
struct buffer                   *buffers;
int                             r, fd = -1;
fd_set                          fds;
struct timeval                  tv;

int setup_capture()
{
    struct v4l2_streamparm ioparam;
    int ret;

    /* O P E N    T H E   D E V I C E */
    //fd = v4l2_open(dev_name, O_RDWR | O_NONBLOCK, 0);
    fd = v4l2_open(dev_name, O_RDWR, 0);
    if (fd < 0) {
        perror("Cannot open device");
        exit(EXIT_FAILURE);
    }


    /* S E T   P A R A M E T E R S */
    CLEAR(fmt);
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = CAP_WIDTH;
    fmt.fmt.pix.height      = CAP_HEIGHT;
    fmt.fmt.pix.pixelformat = FMT;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    xioctl(fd, VIDIOC_S_FMT, &fmt);
    if (fmt.fmt.pix.pixelformat != FMT) {
        printf("Libv4l didn't accept YUYV format. Can't proceed.\n");
        exit(EXIT_FAILURE);
    }
    if ((fmt.fmt.pix.width != CAP_WIDTH) || (fmt.fmt.pix.height != CAP_HEIGHT))
        printf("Warning: driver is sending image at %dx%d\n",
                fmt.fmt.pix.width, fmt.fmt.pix.height);


    /* F R A M E S   P E R   S E C O N D  */
    CLEAR(ioparam);

    ioparam.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioparam.parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
    ioparam.parm.capture.capturemode = V4L2_MODE_HIGHQUALITY;
    ioparam.parm.capture.timeperframe.numerator = 1;//pIntrl->interval.numerator;
    ioparam.parm.capture.timeperframe.denominator = FPS;//pIntrl->interval.denominator;

    if ( -1 == ioctl(fd, VIDIOC_S_PARM, &ioparam)) {
        printf("Error in setting frame rate\n");
        exit(1);
    }

    memset(&ioparam, 0, sizeof ioparam);
    ioparam.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(fd, VIDIOC_G_PARM, &ioparam);
    if (ret < 0) {
        printf("Unable to get frame rate: %d.\n", errno);
        return ret;
    }   

    printf("Current frame rate: %u/%u\n",
            ioparam.parm.capture.timeperframe.numerator,
            ioparam.parm.capture.timeperframe.denominator);


    /* S E T U P   IO */
    CLEAR(req);
    req.count = 5;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    xioctl(fd, VIDIOC_REQBUFS, &req);

    buffers = calloc(req.count, sizeof(*buffers));
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        CLEAR(buf);

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        xioctl(fd, VIDIOC_QUERYBUF, &buf);

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start = v4l2_mmap(NULL, buf.length,
                PROT_READ | PROT_WRITE, MAP_SHARED,
                fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start) {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < n_buffers; ++i) {
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        xioctl(fd, VIDIOC_QBUF, &buf);
    }

	return 0;

}

int capture()
{
#if 0
    do 
    {
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        /* Timeout. */
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        //printf("entering select\n");
        r = select(fd + 1, &fds, NULL, NULL, &tv);
        printf("select done\n");
    } while ((r == -1 && (errno = EINTR)));

    if (r == -1) {
        perror("select");
        return errno;
    }
#endif
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    xioctl(fd, VIDIOC_DQBUF, &buf);

    //printf("Got Image: buf.index=%d, buf.bytesUsed=%d\n", buf.index, buf.bytesused);

	return 0;
}

int close_capture()
{
    /* C L O S E   I O */
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl(fd, VIDIOC_STREAMOFF, &type);
    for (i = 0; i < n_buffers; ++i)
        v4l2_munmap(buffers[i].start, buffers[i].length);

    /* C L O S E   D E V I C E */
    v4l2_close(fd);

	return 0;
}


/* -------- - ------------ - ----------- - ----------- - --------- */




/*    R E N D E R    */
Display *display;
int screen, ret=-1;
Window window;
unsigned long border, background;
GC gc;
XImage *pImage;
struct SwsContext *swsCtx = NULL;
char *outputImage=NULL;
XVisualInfo       vinfo;
XSetWindowAttributes  xswa;
unsigned long     mask;
int           shmem_flag = 0;
XEvent        event;
unsigned int  p_version, p_release, p_request_base, p_event_base, p_error_base, p_num_adaptors;
  XvAdaptorInfo     *ai;
  int       xv_port = -1;
  XvImage       *yuv_image;
  XShmSegmentInfo   yuv_shminfo;



unsigned char *inData[4] = {0};
unsigned int inLine[4] = {0};
unsigned char *outData[4] = {0};
unsigned int outLine[4] = {0};

int setup_render()
{

    /*   O P E N   D I S P L A Y    */
    display = XOpenDisplay(NULL);
    if (display == NULL)
    {   
        printf("Cannot open display\n");
        exit(1);
    }   

    /*   G E T   S C R E E N   */
    screen = DefaultScreen(display);

	 /** find best display */
  if (XMatchVisualInfo(display, screen, 24, TrueColor, &vinfo)) {
    printf(" found 24bit TrueColor\n");
  } else
    if (XMatchVisualInfo(display, screen, 16, TrueColor, &vinfo)) {
      printf(" found 16bit TrueColor\n");
    } else
      if (XMatchVisualInfo(display, screen, 15, TrueColor, &vinfo)) {
    printf(" found 15bit TrueColor\n");
      } else
    if (XMatchVisualInfo(display, screen, 8, PseudoColor, &vinfo)) {
      printf(" found 8bit PseudoColor\n");
    } else
      if (XMatchVisualInfo(display, screen, 8, GrayScale, &vinfo)) {
        printf(" found 8bit GrayScale\n");
      } else
        if (XMatchVisualInfo(display, screen, 8, StaticGray, &vinfo)) {
          printf(" found 8bit StaticGray\n");
        } else
          if (XMatchVisualInfo(display, screen, 1, StaticGray, &vinfo)) {
        printf(" found 1bit StaticGray\n");
          } else {
        printf("requires 16 bit display\n");
        exit (-1);
          }   

    /*   C R E A T E    A    W I N D O W  */ 
  xswa.colormap =  XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
  xswa.event_mask = StructureNotifyMask | ExposureMask;
  xswa.background_pixel = 0;
  xswa.border_pixel = 0;

  mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

  window = XCreateWindow(display, DefaultRootWindow(display),
             0, 0,
             WIN_WIDTH,
             WIN_HEIGHT,
             0, vinfo.depth,
             InputOutput,
             vinfo.visual,
             mask, &xswa);

    /*   S E T    S T A N D A R D   W I N D O W   P R O P E R T I E S   */
    XSetStandardProperties(display, window, "XV Loopback", "XV Loopback", None, NULL, 0, NULL);


    /*   S E L E C T   I N P U T  */
    XSelectInput(display, window, ExposureMask|KeyPressMask|ButtonPressMask|StructureNotifyMask); 


    /*   P U S H    W I N D O W   TO   X - S E R V E R  */
    XMapWindow(display, window);
    //XMoveWindow(display, window, 100, 100);

    //XFlush(display);

/** Wait for map. */
  do {
    XNextEvent(display, &event);
  }
  while (event.type != MapNotify || event.xmap.event != window);


  /* Check if shared memory extensions are available */
  if (XShmQueryExtension(display))
    shmem_flag = 1;

  if (!shmem_flag) {
    printf("no shmem available.\n");
    exit (-1);
  }

 /** query and print Xvideo properties */
  ret = XvQueryExtension(display, &p_version, &p_release, &p_request_base,
             &p_event_base, &p_error_base);
  if (ret != Success) {
	exit(1);
	}

  ret = XvQueryAdaptors(display, DefaultRootWindow(display),
            &p_num_adaptors, &ai);

  if (ret != Success) 
	{
	printf("No adaptor available\n");
	exit(1);
	}

/* Selecting the last adaptor and first port of the last adaptor */
for (i = 0; i < p_num_adaptors; i++) 
	{
       xv_port = ai[i].base_id;
	}

  if (p_num_adaptors > 0)
    XvFreeAdaptorInfo(ai);
  if (xv_port == -1)
    exit (0);

	XvImageFormatValues * imgFormats;
	int numFormats = -1, id;
	char fmtStr[] = "I420";	/* Hard coding format that we need. The string itself is found out by first checking what
								XvListImageFormats returns */
	imgFormats = XvListImageFormats(display, xv_port, &numFormats);

	/* Retrieve the ID of color format that we need */
	for(i=0;i<numFormats;i++)
	{
		if(!strcmp(imgFormats[i].guid, fmtStr))
		{
			id = imgFormats[i].id;
			break;
		}
	}

    /*    C R E A T E    G R A P H I C A L    C O N T E X T  */
    gc = XCreateGC(display, window, 0, 0);

    /*   C R E A T E    I M A G E */
  yuv_image = XvShmCreateImage(display, xv_port, id, 0, WIN_WIDTH, WIN_HEIGHT, &yuv_shminfo);
  yuv_shminfo.shmid = shmget(IPC_PRIVATE, yuv_image->data_size, IPC_CREAT | 0777);
  yuv_shminfo.shmaddr = yuv_image->data = shmat(yuv_shminfo.shmid, 0, 0);
  yuv_shminfo.readOnly = False;

  if (!XShmAttach(display, &yuv_shminfo)) {
    printf("XShmAttach failed !\n");
    exit (-1);
  }

	return 0;
}


int close_render()
{
    /*   F R E E   E V E R Y T H I N G   A N D   C L O S E   D O W N  */
    XFree(pImage);
    pImage = NULL;
    //free(imageData);
    free(outputImage);
    outputImage = NULL;
    XCloseDisplay(display);
	
	return 0;
}


int render()
{
 int       ret, _d;
 unsigned int _w,  _h;;
  Window         _dw;


    /*   P U T   I M A G E */
	XGetGeometry(display, window, &_dw, &_d, &_d, &_w, &_h, &_d, &_d);

	memcpy(yuv_image->data, buffers[buf.index].start, yuv_image->data_size);
      ret = XvShmPutImage(display, xv_port, window, gc, yuv_image,
            0, 0, yuv_image->width, yuv_image->height,
            0, 0, _w, _h, True);

	return 0;
}



int main(int argc, char **argv)
{
    static long last_msg_time = 0;
    static time_t last_msg_sec;
    struct timespec timespec, timespec2;



    setup_capture();
    printf("setup_capture done\n");
    setup_render();
    printf("setup_render done\n");

    /* S T A R T    C A P T U R E */
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    xioctl(fd, VIDIOC_STREAMON, &type);
    printf("capture started\n");


    for (i = 0; i < 3000; i++) {
        /*VIDEO_LOGD("%s: message:%d\n", function, pMessage->MsgId);*/
        clock_gettime(CLOCK_REALTIME, &timespec);
        timespec2 = timespec;
        if (last_msg_time != 0)
        {
            if (timespec.tv_nsec < last_msg_time) { timespec.tv_nsec += 1000000000l; timespec.tv_sec -= 1; }
            printf("MSG %ld:%ld\n", timespec.tv_sec - last_msg_sec, timespec.tv_nsec - last_msg_time);
        }
        last_msg_time = timespec2.tv_nsec;
        last_msg_sec = timespec2.tv_sec;
        capture();
        clock_gettime(CLOCK_REALTIME, &timespec);
        render();
        clock_gettime(CLOCK_REALTIME, &timespec2);
        xioctl(fd, VIDIOC_QBUF, &buf);
        if (timespec2.tv_nsec < timespec.tv_nsec) { timespec2.tv_nsec += 1000000000l; timespec2.tv_sec -= 1; }
        printf("played frame %d %ld:%ld\n", i, timespec2.tv_sec - timespec.tv_sec, timespec2.tv_nsec - timespec.tv_nsec);
    }

    close_capture();
    close_render();
    return 0;
}
