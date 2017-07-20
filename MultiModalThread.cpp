#include <MultiModalThread.h>
#include <yarp/os/LogStream.h>

using namespace yarp::os;
using namesoace yarp::sig;

MultiModalThread::MultiModalThread()
  : semStart(0), senDone(0), interrupted(false) { }

MultiModalThread:~MultiModalThread() { }

void MultiModalThread::run() {
  // Loop until the thread is running.
  while (!isStopping()) {
    // Wait to start the processing.
    semStart.wait();
    if (interrupted) {
      semDone.post();
      return;
    }
    // @TODO: Think of any possible image processing.
    
    // Let other know the thread already processed.
    semDone.post();
  }
}

void MultiModalThread::objectDetection(yarp::os::ImagaOf<yarp::sig::PixelRgb>& image) {
  MultiModalThread::image = &image;
 
  // Signal the thread to start processing.
  semStart.post();
}

void MultiModalThread::objectPointer() {
yarp::sig::Vector elbow_joint(3), wrist_joint(3), hand_vector(3);
    Property prop;
    objectRecognition();
    Bottle *bot = skeletonPort.read();
    Bottle& pos = bot->findGroup("POS");
    //printf("Pos-whole : %s\n", pos.toString().c_str());
    //printf("Pos-size: %d\n", bot->size());
    for (int i=1; i < bot->size(); i++) {
      //printf("Pos[%d]: %d\n", i, bot->get(i).asInt());
    }

    Bottle bottle;
    // Extract skeleton parts.
    bottle = showBottle(*bot);
    if (!bottle.isNull()) {
      printf("Elbow-joint and wrist-joint vectors: %s\n", bottle.toString().c_str());
      for (int i = 0; i < bottle.size(); i++) {
        Value& element_list = bottle.get(i);
        Bottle *lst = element_list.asList();
        for (int j = 0; j < lst->size(); j++) {
          Value& element = lst->get(j);
          if (i == 0) {
            elbow_joint[j] = element.asDouble()/10.0;
          }
          if (i == 1) {
            wrist_joint[j] = element.asDouble()/10.0;
          }
        }
      }

      // Create the hand vector from elbow and wrist joints.
      hand_vector = wrist_joint - elbow_joint;
      printf("hand vector before: %s\n", hand_vector.toString().c_str());

      if (norm(hand_vector) > 0) {
        hand_vector /= norm(hand_vector);
      }
      printf("hand vector normalized: %s\n", hand_vector.toString().c_str());
      // Now we need to create ball centre point as vector.
      yarp::sig::Vector ball_center(3), red_ball(3), purple_ball(3);

      prop.fromConfigFile("object.ini");
      Bottle envBottle;
envBottle = prop.findGroup("robotics");

      red_ball[0] = envBottle.findGroup("red").get(1).asDouble();
      red_ball[1] = envBottle.findGroup("red").get(2).asDouble();
      red_ball[2] = envBottle.findGroup("red").get(3).asDouble();

      purple_ball[0] = envBottle.findGroup("purple").get(1).asDouble();
      purple_ball[1] = envBottle.findGroup("purple").get(2).asDouble();
      purple_ball[2] = envBottle.findGroup("purple").get(3).asDouble();

      VectorOf<yarp::sig::Vector> vector_storage(2);
      //vector_storage[1] = red_ball;
      //vector_storage[2] = purple_ball;

      for (int i = 0; i < vector_storage.size(); ++i) {
        // Create another vector from elbow joint to ball center.
        // This is needed to get dot product of hand_vector.
        if (i == 0) ball_center = red_ball;
        else
          ball_center = purple_ball;
        //printf("Vector storage: %s\n", vector_storage[i].toString().c_str());
        //ball_center = vector_storage[i];
        yarp::sig::Vector elbow_ball_vector(3);
        elbow_ball_vector = ball_center - elbow_joint;


        // We need to take dot product of hand_vector with elbow_ball_vector 
        // and this will give us the distance. If the result is less than zero then the hand vector
        // is pointing away from the ball.
        double distance, radius = envBottle.find("radius").asDouble();
        distance = dot(hand_vector, elbow_ball_vector);

        // Scale hand vector in order to calculate the closest point to the ball.
        yarp::sig::Vector hand_vector_scaled(3), closest_point(3);
        hand_vector_scaled = hand_vector * distance;
        closest_point = elbow_joint + hand_vector_scaled;

        // Check the distance between the closest point and object center. If it is smaller than 
        // the radius then the points is inside the object. That means that the hand points the object with 
        // correct coordinates. 
        printf("Distance: %f\n", distance);
        printf("D2: %f\n", dot(ball_center - closest_point, ball_center - closest_point));
        if (distance > 0 && dot(ball_center - closest_point, ball_center - closest_point) < radius * radius) {
          printf("Pointing correctly: %f\n", distance);
          printf("Ball center before: %s\n", ball_center.toString().c_str());
          igaze->lookAtFixationPoint(ball_center/100.0);// request to gaze at the desired fixation point and wait for reply (sync method)
          printf("Ball center after: %s\n", (ball_center/100.0).toString().c_str());
          igaze->waitMotionDone();
          color_code = RED;
        }
      }
    }
}

void MultiModalThread::wait() {
  semDone.wait();
}

void MultiModalThread::interrupt() {
  interrupted = true;
  semStart.post();
}
