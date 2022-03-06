#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetColor(0);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->font_size = 55;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	string word = "ELDENRING";
	int sample_count = 30;
	auto base_location = glm::vec3(0, 0, 0);

	for (int r = 0; r < 15; r++) {

		auto radius = 350 - r * 10;

		auto angle_x = ofMap(ofNoise(r * 0.015 - ofGetFrameNum() * 0.006), 0, 1, -PI * 1, PI * 1);
		auto angle_y = ofMap(ofNoise(r * 0.015 - ofGetFrameNum() * 0.006), 0, 1, -PI * 1, PI * 1);
		auto angle_z = ofMap(ofNoise(r * 0.015 - ofGetFrameNum() * 0.006), 0, 1, -PI * 1, PI * 1);

		auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
		auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
		auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

		for (int i = 0; i < 50; i++) {

			auto rotation = glm::rotate(glm::mat4(), (i * 7.2f) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
			auto noise_location = glm::vec4(this->font_size * 0.5, base_location.y + this->font_size * -0.5, radius, 0) * rotation;
			noise_location = noise_location * rotation_z * rotation_y * rotation_x;

			int word_index = (int)ofMap(ofNoise(noise_location.x * 0.0001, noise_location.y * 0.0001 - ofGetFrameNum() * 0.005, noise_location.z * 0.0001), 0, 1, 0, word.size() * 2) % word.size();

			ofPath chara_path = this->font.getCharacterAsPoints(word[word_index], true, false);
			vector<ofPolyline> outline = chara_path.getOutline();

			ofFill();
			ofSetColor(255);
			ofBeginShape();
			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

				if (outline_index != 0) { ofNextContour(true); }

				auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
				for (auto& vertex : vertices) {

					auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 350);
					auto rotation = glm::rotate(glm::mat4(), (i * 7.2f) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
					location = glm::vec4(location, 0) * rotation;
					location += base_location;
					location = glm::vec4(location, 0) * rotation_z * rotation_y * rotation_x;

					location = glm::normalize(location) * radius;

					ofVertex(location);
				}
			}
			ofEndShape();

			ofNoFill();
			ofBeginShape();
			ofSetColor(0);
			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

				if (outline_index != 0) { ofNextContour(true); }

				auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
				for (auto& vertex : vertices) {

					auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 350);
					auto rotation = glm::rotate(glm::mat4(), (i * 7.2f) * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
					location = glm::vec4(location, 0) * rotation;
					location += base_location;
					location = glm::vec4(location, 0) * rotation_z * rotation_y * rotation_x;

					location = glm::normalize(location) * radius;

					ofVertex(location);
				}
			}
			ofEndShape(true);
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}