/**
 * @file gpx.h
 * @brief Header file for GPX writing
 */

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

#ifndef GPX_H
#define GPX_H

#include <iostream>
#include <fstream>
#include <string>

/**
 * \brief   GPX Object
 *
 * \details Allows opening, adding trackpoints, and adding extension details
 */
class GPX
{
  private:
    GPX(const GPX&);
    GPX& operator=(const GPX&);

    std::string _file;
    std::ofstream _stream;
    bool _inTrkPt = false;
    bool _inExtensions = false;
  public:
    /// Constructor
//    GPX(std::string file) :
//      _file(file), _stream(file, std::ofstream::app) {}
    GPX(){}
    /**
     * \brief   Function to begin next trackpoint
     *
     * \details 
     *
     * \note    
     *
     * \param[in]     arg1     desc
     * \param[in]     arg2     desc
     *
     * \return   True if successful, or FALSE if error.
     */
    bool startTrkPt(std::string lat, std::string lon, std::string ele, std::string gpsTime);
    bool startTrkPt(float lat, float lon, float ele, std::string gpsTime);

    /**
     * \brief   Function to insert next trackpoint
     *
     * \details Internally calls startTrkPt then endTrkPt
     *
     * \note    
     *
     * \param[in]     arg1     desc
     * \param[in]     arg2     desc
     *
     * \return   True if successful, or FALSE if error.
     */
    bool addTrkPt(std::string lat, std::string lon, std::string ele, std::string gpsTime);
    bool addTrkPt(float lat, float lon, float ele, std::string gpsTime);

    /**
     * \brief   Add extension to current trackpoint
     *
     * \details 
     *
     * \note    
     *
     * \param[in]     name     Name of extension tag
     * \param[in]     value    Value
     *
     * \return   True if successful, or FALSE if error.
     */
    bool addExtension(std::string name, std::string value);

    /**
     * \brief   Function to end current trackpoint
     *
     * \details 
     *
     * \note    
     *
     * \param[in]     arg1     desc
     * \param[in]     arg2     desc
     *
     * \return   True if successful, or FALSE if error.
     */
    bool endTrkPt();
    
    /**
     * \brief   Function to initialize GPX file
     *
     * \details Creates, and writes basic header for GPX file
     *
     * \note    
     *
     * \param[in]     arg1     desc
     * \param[in]     arg2     desc
     *
     * \return   True if successful, or FALSE if error.
     */
    bool initialize(std::string file);

    /**
     * \brief   Finish and close GPX file
     *
     * \details Inserts footer tags and closes ofstream
     *
     * \note    
     *
     * \param[in]     arg1     desc
     * \param[in]     arg2     desc
     *
     * \return   True if successful, or FALSE if error.
     */
    bool close();
};
#endif
