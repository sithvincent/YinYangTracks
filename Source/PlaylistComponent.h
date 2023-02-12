#pragma once

#include <JuceHeader.h>
#include <vector>


class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener
{
    public:
        //===============Constructor and Background Draw Function ======================
        PlaylistComponent();
        ~PlaylistComponent() override;

        // Draw functions
        void paint (juce::Graphics&) override;
        void resized() override;


        //===============Functions inherited from TableListBoxModel======================
        
        // Returns number of rows in the table.
        int getNumRows() override;

        // Paints each row of the table (row background). Arguments are 'auto-passed in'.
        void paintRowBackground(Graphics &,
                                int rowNumber,
                                int width,
                                int height,
                                bool rowIsSelected) override;

        // Paints/fills up the individual cell in each row. Arguments are 'auto-passed in'.
        void paintCell( Graphics &,
                        int rowNumber,
                        int columnId,
                        int width,
                        int height,
                        bool rowIsSelected) override;

        // Dictates what happens when you click on a cell. Non abstract.
        void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;



        // Create or update a custom component in every cell. Non abstract.
        // Called whenever a custom component might need to be updated e.g.when the table is changed.
        // Pointer to the component of each cell (existingComponentToUpdate) is auto passed in.
        Component* refreshComponentForCell( int rowNumber,
                                            int columnId,
                                            bool isRowSelected,
                                            Component* existingComponentToUpdate) override;


        void buttonClicked(Button* button) override;

        void addEntry(String, double, URL);
        void printTitles();
        URL urlToLoad;
        bool loadMusicIntoDeck;


    private:
        // Main table object, uses TableListBoxModel helper functions to get information about
        // the data for the table and the way that data should be drawn. 
        // Need to register TableListBoxModel functions with this component.
        TableListBox tableComponent;


        // Vector to store the tracktitles
        std::vector <String> trackTitles;
        std::vector <double> trackLengths;
        std::vector <URL> trackURLs;

        // MACRO
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
