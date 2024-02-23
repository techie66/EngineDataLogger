/*
    EngineDataLogger
    Copyright (C) 2018-2022  Jacob Geigle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "gpx.h"

bool GPX::startTrkPt(float lat, float lon, float ele, std::string gpxTime)
{
  return startTrkPt(std::to_string(lat), std::to_string(lon), std::to_string(ele), gpxTime);
}
bool GPX::startTrkPt(std::string lat, std::string lon, std::string ele, std::string gpxTime)
{
  if (_stream.is_open() && !_inTrkPt) {
    _stream << "      <trkpt lat=\"" << lat << "\" lon=\"" << lon << "\">\n" \
            << "        <ele>" << ele << "</ele>\n" \
            << "        <time>" << gpxTime  << "</time>\n";
    _inTrkPt = true;
    return true;
  }
  return false;
}

bool GPX::addExtension(std::string name, std::string value)
{
  if (_stream.is_open() && _inTrkPt) {
    if (!_inExtensions) {
      _stream << "        <extensions>\n";
      _inExtensions = true;
    }
    _stream << "          <" << name << ">" << value << "</" << name << ">\n";
    return true;
  }
  return false;
}

bool GPX::endTrkPt()
{
  if (_stream.is_open() && _inTrkPt) {
    if (_inExtensions) {
      _stream << "        </extensions>\n";
      _inExtensions = false;
    }
    _stream << "      </trkpt>\n";
    _inTrkPt = false;
    return true;
  }
  return false;
}

bool GPX::addTrkPt(float lat, float lon, float ele, std::string gpxTime)
{
  if (startTrkPt(std::to_string(lat), std::to_string(lon), std::to_string(ele), gpxTime))
    if (endTrkPt())
      return true;
  return false;
}

bool GPX::addTrkPt(std::string lat, std::string lon, std::string ele, std::string gpxTime)
{
  if (startTrkPt(lat, lon, ele, gpxTime))
    if (endTrkPt())
      return true;
  return false;
}

bool GPX::initialize(std::string file)
{
  _stream = std::ofstream(file, std::ofstream::app);
  _file = file;
  if (_stream.is_open()) {
    _stream << "<?xml version=\"1.0\" encoding=\"utf-8\"?><gpx creator=\"Engine Data Logger\" version=\"1.1\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd http://www.garmin.com/xmlschemas/WaypointExtension/v1 http://www8.garmin.com/xmlschemas/WaypointExtensionv1.xsd http://www.garmin.com/xmlschemas/TrackPointExtension/v1 http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd http://www.garmin.com/xmlschemas/GpxExtensions/v3 http://www8.garmin.com/xmlschemas/GpxExtensionsv3.xsd http://www.garmin.com/xmlschemas/ActivityExtension/v1 http://www8.garmin.com/xmlschemas/ActivityExtensionv1.xsd http://www.garmin.com/xmlschemas/AdventuresExtensions/v1 http://www8.garmin.com/xmlschemas/AdventuresExtensionv1.xsd http://www.garmin.com/xmlschemas/PressureExtension/v1 http://www.garmin.com/xmlschemas/PressureExtensionv1.xsd\" xmlns=\"http://www.topografix.com/GPX/1/1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:wptx1=\"http://www.garmin.com/xmlschemas/WaypointExtension/v1\" xmlns:gpxtrx=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3\" xmlns:gpxtpx=\"http://www.garmin.com/xmlschemas/TrackPointExtension/v1\" xmlns:gpxx=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3\" xmlns:trp=\"http://www.garmin.com/xmlschemas/TripExtensions/v1\" xmlns:adv=\"http://www.garmin.com/xmlschemas/AdventuresExtensions/v1\" xmlns:prs=\"http://www.garmin.com/xmlschemas/PressureExtension/v1\">\n";
    _stream << "  <trk>\n    <name>Test</name>\n    <trkseg>\n";
    return true;
  }
  return false;
}

bool GPX::close()
{
  if (_stream.is_open()) {
    if (_inTrkPt)
      endTrkPt();
    _stream << "    </trkseg>\n";
    _stream << "  </trk>\n";
    _stream << "</gpx>\n";
    _stream.close();
    return true;
  }
  return false;
}
