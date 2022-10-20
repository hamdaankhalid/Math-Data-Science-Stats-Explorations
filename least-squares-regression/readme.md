# Solving Simple Linear Regression From Scratch

## Pre-requisite terms and concepts:
- Residual sum of squares: The sum of all errors where error is defined as => (predicted points - expected points) squared.
- Line: Y = SLOPE * X + INTERCEPT

## Background:
Everytime I look at statistics books and reach the linear regression topic, I get told about the Residual sum of squares, and how finding the linear regression  modelling process involves finding a line that reduces the Residual sum of squares to the least possible values. This time I decided I will stare at it till I figure out an algorithm that actually finds a line to reduce residual sum of squares, without any help or intuition from any existing algorithms.

## Intuition:

### Finding an Anchor
After drawing a scatterplot in my office whiteboard I decided to stare at it and imagine a line that would reduce the residual sum of squares. I noticed that if I were to just freehand estimate such a line it would be passing through the center of the clouds in the scatter plot. This lead me to the idea of finding the "centroid" in our scatterplot, this was pure intuition and luck in retrospect, I had a hypothesis and then I whipped up 10 scatterplots and confirmed my belief that the best line would always pass through our centroid. 
After figuring out the role of the centroid, I knew that the next problem here on would be understanding the tilt/slope that a line that passes through the centroid would have. This line would have the centroid as an axis and would rotate on it. Our Centroid would be the anchor!

### Weird stars and their inverted gravitational pull....
Each point on our scatter plot tilts our line towards it. I visualized it such that it has it's own gravitational pull of some sort! Except unlike real stars, the magnitude of this pull increases the further away it is from the line. I then visualized that there is a line parallel to our x-axis that passes through our centroid. The line has 4 directions were it is feeling force from. top-left of the centroid, bottom-left of the centroid, top-right of the centroid, and bottom-right of the centroid. Visualize it like the pic below!

- TODO: INSERT PIC

Points that lie on the same horizontal side of the centroid and are on the same vertical side, can be combined as to find the total force with which they pull the line. e.g. If every point in quadrant A pulls the line up by x amount, then the combined force that quadrant A is being pulled by can be represented with sum of all xi where xi represents the force exerted by point i to pull the line towards itself, and x belongs to quadrant A.

Points that lie on the same horizontal side of the centroid and are vertically opposite to each other such as the points in quadrants A and D or points in quadrants B and C, are attracting the line in oppposing directions. e.g. points from quadrant A pull the line up and points from quadrant D pulls the line down. So the net force on the left side of the centroid can be calculated as: Total force exerted by quadrant A - Total force exerted by quadrant D.

Now we have a left side of the centroid and a right side of the centroid. The horizontal line sits on an anchor that acts like a fulcrum and the left and right side forces tilt it up or down from their side! If we imagine a beam with a fulcrum with a set of weights on each side, it is bound to tilt down from the side if the heavier weight. If the weights are equal then the beam will not tilt anywhere. When the force on both sides of the beam ris equal it reaches an equilibrium. Our line has forces being exerted from the left and right side of the centroid, whichever side  pulls it with more force it will tilt towards it. So the Net effect on the line can be described as the following:
NET FORCE OF PULL = TOTAL FORCE PULLED WITH ON THE RIGHT - TOTAL FORCE PULLED WITH ON THE LEFT
If the net force of pull is greater than 0 then we will pull the line up from the right side of the centroid while keeping it anchored on the fulcrum. 
If the net force of pull is less than 0 then we will tilt the line down from the right side of the entroid while keeping it anchored on the fulcrum.
If the net force is equal to 0 then the line is at equilibrium and the graviational pull is satisfied on left and right sides simultaneously.

### Cool story about the force and points but can you quantify it!?
All of this is great but the hand-wavy idea of the force applied by a point on the graph meant nothing till I could quantify it. I used the idea of our fulcrum along with force of pulling to look at how levers and torque based physics works. Every point's pull has force equal to the distance from it's y coordinate to the y of our line. It hits pulls our line up along the y-axis at an angle at 90 degrees, when our line is initially horizontal, if our line was tilted it would then be pulling as at an angle of THETA degrees (where theta would be the angle between the y axis and the line). By observation I also realized that the values on the extreme lefts and extreme rights of our scatterplot would be very influential in deciding the tilt/slope of our line. This lead me to realizing that the force exerted by each point can be represented as TORQUE ( force * length from fulcrum * sin(THETA) ) in the following form:
Torque from 1 point = (Y of point coordinate) - (Y on the line for same X as point) * (X of centroid(fulcrum X) - X of point coordinate) * sin(THETA)

### Bringing it all together and binary searching for equilibrium
The net torque on our line gives us an idea of which side to tilt the line. If we start with a horizontal line our theta will be set to 90 degrees. Theta can range anywhere from 0 to 180 degrees. Theta at 0 would be our line is veritcal with the theoretical head of the line on the of the y-axis. Theta of 90 means the line is parallel to x-axis with the theoretical head of our line on the right side. Theta of 180 means the line is vertical with the theoretical head at the top of the y-axis. Our target theta will give us a net torque as 0, any theta lesser than zero means we can tilt our line up from the right side, and any theta higher than zero would mean we can tilt our line down from the right side. That's right we have brought everything together and we are binary searching!
The net torque at each iteration in our binary search algorithm can be found out using the following formula:
Net Torque = Net force on right - net force on left = (Sum of torque by each point in quadrant A - Sum of torque by each poin in quadrant D) - (Sum of torque by each point in quadrant B - Sum of torque by each poin in quadrant C).
Eventually we will converge to a point where our line's theta will be approximately equal to 0. At this point we know our theta, and we know of one point that lies on our line our centroid. Using the theta we can derive our slope to be equal to cotanget(theta), with slope and one point on the line (centroid) we can get our intercept from the following formula : intercept = (Y of centroid) - (slope* X of centroid). Now we have a slope and an intercept which means we can represent the best fit line for linear regression.
