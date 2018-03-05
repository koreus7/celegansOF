#pragma once
#include "catch.h"
#include "../serializable.h"

int theAnswer() { return 42; }

class MockSerializable : public ISerializable
{
public:
    int childInt = 24;
    int mockInt = 12;

    void serialize(ofxJSONElement &root) const override
    {
        ofxJSONElement childObject = ofxJSONElement(Json::objectValue);

        childObject["childInt"] = childInt;

        root["mockInt"] = mockInt;
        root["child"] = childObject;

    }

    void deserialize(const ofxJSONElement &root) override
    {
        mockInt = root["mockInt"].asInt();
        childInt = root["child"]["childInt"].asInt();
    }

    int getUniqueId() const override
    {
        return 111;
    }
};

TEST_CASE( "Serializes to JSON Element" ) {

    MockSerializable mock;

    ofxJSONElement root;

    mock.serialize(root);

    REQUIRE( root["mockInt"].asInt() == 12 );
    REQUIRE( root["child"]["childInt"].asInt() == 24 );
}


