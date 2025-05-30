import cv2
import numpy as np
from elements_yolo import OBJ_DETECTION
import time

Object_classes = ['weed']
print(len(Object_classes))

Object_colors = list(np.random.rand(40, 3) * 255)
Object_detector = OBJ_DETECTION('best.pt', Object_classes)

# This line seems out of place — it likely belongs elsewhere when working with contours
# contours, hierarchy = cv2.findContours(image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

cap = cv2.VideoCapture(1)

if cap.isOpened():
    window_handle = cv2.namedWindow("Camera", cv2.WINDOW_AUTOSIZE)

    while cv2.getWindowProperty("Camera", 0) >= 0:
        ret, frame = cap.read()

        if ret:
            # Detection process
            objs = Object_detector.detect(frame)

            # Plotting
            for obj in objs:
                label = obj['label']
                score = obj['score']

                if label == 'weed':
                    print(score)
                    print("weed detected")

                    [[xmin, ymin], [xmax, ymax]] = obj['bbox']
                    color = Object_colors[Object_classes.index(label)]

                    frame = cv2.rectangle(frame, (xmin + 50, ymin + 50), (xmax + 50, ymax + 50), color, 2)
                    frame = cv2.putText(frame, f'{label} ({str(score)})', (xmin + 50, ymin + 50),
                                        cv2.FONT_HERSHEY_SIMPLEX, 0.75, color, 1, cv2.LINE_AA)

            cv2.imshow("Camera", frame)

            keyCode = cv2.waitKey(1)
            if keyCode == ord('q'):
                break

    cap.release()
    cv2.destroyAllWindows()
    # serialcomm.close()  # Uncomment if you are using serial communication
else:
    print("Unable to open camera")
