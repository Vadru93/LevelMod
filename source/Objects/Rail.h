#pragma once
#ifndef RAIL_H
#define RAIL_H
#include "Script\Script.h"
#include "Script\Node.h"


#define MAX_RAIL_LINKS 4
#define MAX_RAIL_NODES 10000

vector<RailNode> pointy_rails;

typedef void(__cdecl* const pRail_ComputeBB)(Vector& pos1, Vector& pos2, Vector& bb_min, Vector& bb_max);
#define Rail_ComputeBB pRail_ComputeBB(0x004957F0)
extern vector<ColouredVertex> bbox_rails;

class RailNode
{
    friend class RailManager;

    enum class Flags : WORD
    {
        DEFAULT_LINE = 1,
        LIP_OVERRIDE = 2,
        ONLY_CLIMBING = 4,
        LADDER = 8,
    };
protected:
    Vector vBBMin;;
    Vector vBBMax;
    Vector vPos;
    //RailNode* pNext;
    RailNode* pNextLink;
    RailNode* pPrevLink;
    signed short node;
    signed short link;
    BYTE terrain;
    bool bActive;
    WORD flags;

private:
    void SetActive(bool active)
    {
        bActive = active;
    }

    bool GetFlag(RailNode::Flags flag)
    {
        return (flags & (WORD)flag);
    }

    void SetFlag(RailNode::Flags flag)
    {
        flags |= (WORD)flag;
    }

    void SetFlag(RailNode::Flags flag, bool set)
    {
        if (set)
            flags |= (WORD)flag;
        else
            flags &= ~(WORD)flag;
    }

    bool IsActive()
    {
        return bActive;
    }

public:

    RailNode(const Vertex pos)
    {
        *(Vertex*)&vPos = pos;
        vPos.w = 1.0f;
        //Rail_ComputeBB(vPos, vPos, vBBMin, vBBMax);
        vBBMin.x = vPos.x - 5.0f;
        vBBMin.y = vPos.y - 5.0f;
        vBBMin.z = vPos.z - 5.0f;
        vBBMax.x = vPos.x + 5.0f;
        vBBMax.y = vPos.y + 5.0f;
        vBBMax.z = vPos.z + 5.0f;
        pNextLink = NULL;
        pPrevLink = NULL;
        node = -1;
        link = -1;
        terrain = 0;
        bActive = true;
        flags = 0;
    }
    // Added by Ken.
// Returns true if the passed Node is on the same rail as the rail node.
// Note, with the "merging" rails, where two nodes can link to
// a new node, then this is not guarenteed to work
//  
// Given that the rails can now form a "loop with a tail"
// we now detect loops by traversign the list with two pointers
// one moving at half the speed of the other

    bool ProbablyOnSameRailAs(int SearchNode)
    {


        // First check if this node is the required node.
        if (node == SearchNode || (pNextLink && pNextLink->node == SearchNode) || (pPrevLink && pPrevLink->node == SearchNode))
        {
            // Well that was easy.
            return true;
        }


        // MICK:  Modified to return true only if on the same rail segment

        //return false;




        const RailNode* p_trailing = this;
        bool		advance_trailing = false;

        // Scan forwards.
        RailNode* pNode = pNextLink;
        while (pNode)
        {
            if (pNode->node == SearchNode)
            {
                // Found it.
                return true;
            }
            if (pNode == p_trailing)
            {
                // We've got back where we started without finding
                // the node, so return false.
                return false;
            }
            pNode = pNode->pNextLink;

            // Advance the trailing node every other time
            // we advance the search node
            if (advance_trailing)
            {
                p_trailing = p_trailing->pNextLink;
            }
            advance_trailing = !advance_trailing;
        }

        p_trailing = this;
        advance_trailing = false;

        // Didn't find anything that way, so now scan backwards.
        pNode = pPrevLink;
        while (pNode)
        {
            if (pNode->node == SearchNode)
            {
                // Found it.
                return true;
            }
            if (pNode == p_trailing)
            {
                // We've got back where we started without finding
                // the node, so return false.
                return false;
            }
            pNode = pNode->pPrevLink;
            // Advance the trailing node every other time
            // we advance the search node
            if (advance_trailing)
            {
                p_trailing = p_trailing->pPrevLink;
            }
            advance_trailing = !advance_trailing;

        }

        return false;
    }
    int GetNode() const
    {
        return (int)node;
    }
    DWORD GetTerrain() const
    {
        return (DWORD)terrain;
    }
    const RailNode* __restrict GetNextLink() const
    {
        return pNextLink;
    }
    const RailNode* __restrict GetPrevLink() const
    {
        return pPrevLink;
    }
    int	Side(const Vertex& vel) const
    {
        Vertex Start = *(D3DXVECTOR3*)&vPos;
        Vertex End = *(D3DXVECTOR3*)&pNextLink->vPos;
        Game::skater->Store();


        // Find a point "Mid", which is 1/4 of the way along the rail   
        // and lowered six inches
        Vertex Mid = (Start + End) / 2.0f;			// Half way along
        Mid = Start + ((Mid - Start) / 2.0f);		    // quarter of the way along	
        Mid[Y] -= 6.0f;									// lowered a bit

    // Find a vector "Side" which is horizontal, perpendicular to the rail, and 4 inches long	
        Vertex Side = End - Start;	   			// vector along the rail
        Side[Y] = 0.0f;								// horizontal
        Side.Normalize(4.0f);						// this is half the width of the thickest rail.  Was 4.0, changed for Tokyo mega funbox
        float temp = Side[X];			 				// make perpendicular to rail
        Side[X] = Side[Z];
        Side[Z] = -temp;


        bool	left = false;
        bool	right = false;

        // if collision above me, left to right
        D3DXVECTOR3 temp1 = Mid + Side;
        D3DXVECTOR3 temp2 = Mid - Side;
        Game::skater->SetRay(temp1, temp2);
        if (Game::skater->CollisionCheck())
        {
            left = true;
        }

        // if collision above me, right to left
        Game::skater->FlipDirection();
        if (Game::skater->CollisionCheck())
        {
            right = true;
        }
        else
        {
        }

        int side = 0;
        if (left && !right)
        {
            side = -1;
        }
        else
        {
            if (right && !left)
            {
                side = 1;
            }
        }

        // flip is we are going the opposite direction on the rail
        const D3DXVECTOR3 temp3 = (End - Start);
        if (side && (D3DXVec3Dot(&vel, &temp3) < 0.0f))
        {
            side = -side;
        }

        Game::skater->Restore();
        return side;
    }

};

class	RailLinks
{
    friend class RailManager;
    signed short m_link[MAX_RAIL_LINKS];	// link numbers	
};

struct Line2d
{
    Vector start;
    Vector end;

    Line2d()
    {

    }
};

static DWORD current_node;
static RailNode* temp_nodes = NULL;
static RailNode* mp_nodes = NULL;
static RailLinks* temp_links;
static DWORD numRailNodes;
static DWORD numNodes;
static RailNode** first_node;
static DWORD EndOfRail;

class ParkEditor
{
    BYTE unk[0x60];
    bool bUsingCustomPark;
public:
    __inline static ParkEditor* const __restrict Instance()
    {
        return *(ParkEditor**)0x008E1FEC;
    }

    __inline bool UsingCustomPark()
    {
        return bUsingCustomPark;
    }
};
extern vector<SuperSector*> PointyObjects;

class RailManager
{
public:


    static void Initialize()
    {
        Slerp::m_last_wallplant_time_stamp.QuadPart = 0;
        temp_nodes = NULL;
        temp_links = NULL;
        current_node = 0;
        numRailNodes = 0;
        numNodes = 0;
        first_node = NULL;
    }

    static void Cleanup()
    {
        if(first_node)
            *first_node = NULL;
        if (mp_nodes)
            freex(mp_nodes);
        mp_nodes = NULL;
        if (temp_nodes)
            freex(temp_nodes);
        temp_nodes = NULL;

        if(PointyObjects.size())
          PointyObjects.clear();
        if(bbox_rails.size())
          bbox_rails.clear();
        if(pointy_rails.size())
          pointy_rails.clear();
    }

    static void AllocateTempRailData(RailNode** first)
    {
        Node::UpdateNodeArray();//00419BC9
        current_node = 0;
        numRailNodes = 0;
        numNodes = 0;



        Game::skater = Skater::UpdateSkater();
        if(Game::skater)
            Game::skater->Store();

        CArray* node_array = Node::GetNodeArray();
        numNodes = node_array->GetNumItems();
        for (int i = 0; i < numNodes; i++)
        {
            CStruct* pStruct = node_array->GetStructure(i);

            DWORD class_checksum;
            if (pStruct->GetChecksum(Checksums::Class, &class_checksum) && !SkateMod::ShouldBeAbsentNode(pStruct))
            {
                if ((class_checksum == Checksum("RailNode") || class_checksum == Checksum("ClimbingNode")))
                {
                    numRailNodes++;
                }
                else if (class_checksum == Checksums::EnvironmentObject)
                {
                    DWORD name;
                    if (pStruct->GetChecksum(Checksums::Name, &name) && GameState::GotSuperSectors && Game::skater)
                    {
                        SuperSector* sector = SuperSector::GetSuperSector(name);
                        if (sector)
                        {
                            //Get the width and height
                            float x_width = sector->bboxMax.x - sector->bboxMin.x;
                            float height = sector->bboxMax.y - sector->bboxMin.y;
                            float z_width = sector->bboxMax.z - sector->bboxMin.z;

                            //Super math... probably is a better way to do this
                            if (x_width < 35.0f && x_width > 3.0f && z_width < 35.0f && z_width > 3.0f && height > 15.0f)
                            {

                                //Now we found a narrow object, but we still need to check if there is another object ontop of it that will make the object unaccessable
                                //So we will make raytracing from object top position to slightly above and vice versa to combat CCW

                                //Get the middle topmomst point of the object, not 100% true but seems to be true enough
                                D3DXVECTOR3 top;
                                top.x = (sector->bboxMax.x + sector->bboxMin.x) / 2.0f;
                                top.y = sector->bboxMax.y;
                                top.z = (sector->bboxMax.z + sector->bboxMin.z) / 2.0f;

                                D3DXVECTOR3 end = top;
                                //Set end point sligthly above
                                end.y += 50.0f;
                                Game::skater->SetRay(top, end);
                                //Ignore hollow collision
                                if (Game::skater->CollisionCheck())
                                    continue;

                                //We did not collide, but there still may be an object ontop of this object that has the same exact position as the topmost point
                                //So we need to check slightly below the topmost point
                                if (height > 20.0f)
                                    top.y -= 20.0f;
                                else
                                    top.y -= 8.0f;
                                Game::skater->SetRay(top, end);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name && Game::skater->GetHitPoint()->y >= top.y)
                                    continue;

                                //Now we need to check again in reverse order to take care of CCW
                                Game::skater->SetRay(end, top);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name && Game::skater->GetHitPoint()->y >= top.y)
                                    continue;

                                //Now let's check in circle around, should use boundingsphere collision checking instead of raytracing here
                                top.y = sector->bboxMax.y + 1.0f;
                                end.y = top.y;
                                end.x += 50.0f;
                                Game::skater->SetRay(top, end);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name)
                                    continue;

                                end.z += 50.0f;
                                Game::skater->SetRay(top, end);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name)
                                    continue;

                                end.x -= 50.0f;
                                Game::skater->SetRay(top, end);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name)
                                    continue;

                                end.x -= 50.0f;
                                Game::skater->SetRay(top, end);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name)
                                    continue;

                                end.z -= 50.0f;
                                Game::skater->SetRay(top, end);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name)
                                    continue;

                                end.z -= 50.0f;
                                Game::skater->SetRay(top, end);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name)
                                    continue;

                                end.x += 50.0f;
                                Game::skater->SetRay(top, end);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name)
                                    continue;

                                end.x += 50.0f;
                                Game::skater->SetRay(top, end);
                                if (Game::skater->CollisionCheck() && Game::skater->GetCollisionName() != sector->name)
                                    continue;

                                //We have a narrow object that seems to be accessible
                                for (auto j = 0; j < sector->numVertices; j++)
                                {
                                    //Decrease b and g value, keep r value = make object more red
                                    SuperSector::Color* colors = sector->GetColors();
                                    colors[j].g *= 0.2f;
                                    colors[j].b *= 0.2f;
                                }

                                //Tell engine to update the VertexBuffer to vram
                                sector->Update();
                                float highestPoint = -FLT_MAX;
                                for (DWORD j = 0; j < sector->numVertices; j++)
                                {
                                    if (sector->vertices[j].y > highestPoint)
                                        highestPoint = sector->vertices[j].y;
                                }

                                top.y = highestPoint + 1.0f;
                                _printf("Auto-gen RailNode Obj %s pos %f %f %f\n", FindChecksumName(sector->name), top.x, top.y, top.z);
                                pointy_rails.push_back(RailNode(top));
                                pointy_rails.back().link = i;
                                PointyObjects.push_back(sector);
                            }
                        }
                    }
                }
            }
        }
        if (mp_nodes)
            freex(mp_nodes);
        if (temp_nodes)
            freex(temp_nodes);

        if(Game::skater)
            Game::skater->Restore();


        _printf("NumRails %d<\n", numRailNodes);

        if (numRailNodes)
        {
            temp_nodes = (RailNode*)mallocx(sizeof(RailNode) * (numRailNodes+1));
            if (!temp_nodes)
                MessageBox(0, 0, 0, 0);
            EndOfRail = (DWORD)&temp_nodes[numRailNodes];
            temp_links = (RailLinks*)mallocx(sizeof(RailLinks) * numRailNodes+1);
            if (!temp_links)
                MessageBox(0, 0, 0, 0);

            first_node = first;
            /*if (*first)
                freex(*first);*/
            *first = temp_nodes;
            //temp_nodes[0].pNext = NULL;
            temp_nodes[0].pNextLink = NULL;
            temp_nodes[0].pPrevLink = NULL;
        }
        else if (*first)
        {
            RailNode* node = *first;
            //node->pNext = NULL;
            node->pNextLink = NULL;
            node->pPrevLink = NULL;
        }

    }


#define EPS  0.00001f

    static bool LineLineIntersect(Line2d& l1, Line2d& l2, Vertex* pa, Vertex* pb, float* mua, float* mub, bool clamp = true)
    {

        Vertex& p1 = *(Vertex*)&l1.start;
        Vertex& p2 = *(Vertex*)&l1.end;
        Vertex& p3 = *(Vertex*)&l2.start;
        Vertex& p4 = *(Vertex*)&l2.end;

        Vertex p13, p43, p21;

        float d1343, d4321, d1321, d4343, d2121;
        float numer, denom;

        p13 = p1 - p3;
        p43 = p4 - p3;

        if (fabsf(p43[X]) < EPS && fabsf(p43[Y]) < EPS && fabsf(p43[Z]) < EPS)
            return(false);

        p21 = p2 - p1;

        if (fabsf(p21[X]) < EPS && fabsf(p21[Y]) < EPS && fabsf(p21[Z]) < EPS)
            return(false);

        d1343 = D3DXVec3Dot((D3DXVECTOR3*)&p13, (D3DXVECTOR3*)&p43);
        d4321 = D3DXVec3Dot((D3DXVECTOR3*)&p43, (D3DXVECTOR3*)&p21);
        d1321 = D3DXVec3Dot((D3DXVECTOR3*)&p13, (D3DXVECTOR3*)&p21);
        d4343 = D3DXVec3Dot((D3DXVECTOR3*)&p43, (D3DXVECTOR3*)&p43);
        d2121 = D3DXVec3Dot((D3DXVECTOR3*)&p21, (D3DXVECTOR3*)&p21);

        denom = d2121 * d4343 - d4321 * d4321;
        if (fabsf(denom) < EPS)
            return(false);
        numer = d1343 * d4321 - d1321 * d4343;

        *mua = numer / denom;
        if (clamp)
        {
            *mua = Clamp(*mua, 0.0f, 1.0f);
        }
        *mub = (d1343 + d4321 * (*mua)) / d4343;

        if (clamp)
        {
            *mub = Clamp(*mub, 0.0f, 1.0f);
        }

        *pa = p1 + (*mua * p21);
        *pb = p3 + (*mub * p43);

        return(true);
    }

    // see if we can stick to a rail	
    static bool StickToRail(const Vertex& pos1, const Vertex& pos2, Vertex* p_point, RailNode** pp_rail_node, const RailNode* p_ignore_node, float min_dot, int side)
    {

        // Go through all the rail segments, and find the shortest distance to line
        // and there is your rail


        Line2d movement;

        *(Vertex*)&movement.start = pos1;
        *(Vertex*)&movement.end = pos2;

        // find bounding box for skater
        Vector bb_min, bb_max;
        Rail_ComputeBB(movement.start, movement.end, bb_min, bb_max);
        float snap_dist = Physics::Rail_Max_Snap;
        bb_min.Set(bb_min.x - snap_dist, bb_min.y - snap_dist, bb_min.z - snap_dist);
        bb_max.Set(bb_max.x + snap_dist, bb_max.y + snap_dist, bb_max.z + snap_dist);

        float		closest_dist = 10000000.0f;
        RailNode* p_closest_rail = NULL;
        Vertex		closest_point;

        bool	found = false;


        //	CRailNode *pRailNode = mp_first_node;
        //	while (pRailNode)
        for (int check_node = 0; check_node < current_node; check_node++)
        {
            RailNode* const __restrict pRailNode = &mp_nodes[check_node];
            if (!pRailNode->GetFlag(RailNode::Flags::ONLY_CLIMBING) && pRailNode != p_ignore_node && pRailNode->IsActive())
            {
                if (pRailNode->pNextLink)
                {
                    // First do bounding box test, before time-intensive LineLineIntersect test 

                    if (!(bb_min.x > pRailNode->vBBMax.x))
                        if (!(bb_max.x < pRailNode->vBBMin.x))
                            if (!(bb_min.z > pRailNode->vBBMax.z))
                                if (!(bb_max.z < pRailNode->vBBMin.z))
                                    if (!(bb_min.y > pRailNode->vBBMax.y))
                                        if (!(bb_max.y < pRailNode->vBBMin.y))
                                        {
                                            // we have a rail segment with a BB match
                                            // so see if we are close to it 
                                            Line2d segment;
                                            segment.start = pRailNode->vPos;
                                            segment.end = pRailNode->pNextLink->vPos;

                                            if (true)//Rail_ValidInEditor(pRailNode->vPos, pRailNode->pNextLink->vPos))
                                            {
                                                D3DXVECTOR3 p1, p2;
                                                float  f1, f2;
                                                if (LineLineIntersect(movement, segment, (Vertex*)&p1, (Vertex*)&p2, &f1, &f2))
                                                {

                                                    Vertex to_rail = p2 - p1;
                                                    float 	dist = to_rail.Length();
                                                    float	old_dist = dist;

                                                    // calculate the dot product of the
                                                    // the movement and the rail in the XZ plane
                                                    Vertex v1, v2;
                                                    v1 = *(D3DXVECTOR3*)&segment.end - *(D3DXVECTOR3*)&segment.start;
                                                    v2 = pos2 - pos1;
                                                    v1[Y] = 0.0f;
                                                    v2[Y] = 0.0f;
                                                    v1.Normalize();
                                                    v2.Normalize();
                                                    float dot = fabsf(D3DXVec3Dot((D3DXVECTOR3*)&v1, (D3DXVECTOR3*)&v2));

                                                    // if now v2 (the skater's movement vector) is all zero
                                                    // and the dot is 0.0f
                                                    // then that means we were going precisely straght up
                                                    // so we set the dot to 1,
                                                    // as normally (if we we slightly left or right)
                                                    // we would be going along the rail at the top of the pipe
                                                    // in the XZ plane (albeit slowly)
                                                    if (v2[X] == 0.0f && v2[Z] == 0.0f && dot == 0.0f)
                                                    {
                                                        dot = 1.0f;
                                                    }


                                                    dist = dist * (0.122f + 1.0f - dot);		// was (50 + (4096-dist)) on PS1						
                                                    old_dist = old_dist * (2.0f - dot);		// was (8192-dot) on PS1 

                            //						printf ("dot=%.2f, dist=%.2f, old_dist=%.2f, min_dot=%.2f\n",dot,dist,old_dist,min_dot);

                                                    if (side)
                                                    {
                                                        D3DXVECTOR3 vel = pos2 - pos1;
                                                        if (pRailNode->Side(vel) != side)
                                                        {
                                                            dist *= 2.0f;
                                                            //								printf ("side change, dist doubled to %.2f\n",dist);
                                                        }
                                                    }


                                                    if (dist < closest_dist)
                                                    {
                                                        bool close = true;
                                                        // if we have a maximum dot, then check we don't go over it
                                                        if (min_dot != 1.0f)
                                                        {
                                                            if (fabsf(dot) < min_dot)
                                                            {
                                                                close = false;
                                                            }
                                                        }
                                                        if (close)
                                                        {
                                                            if (old_dist > snap_dist)
                                                            {
                                                                // there is a good rail, but too far away
                                                                // so kill any rail we've found so far00
                                                                // and make this the new target
                                                                closest_dist = dist;
                                                                found = false;
                                                            }
                                                            else
                                                            {
                                                                // good rail, and close enough
                                                                closest_dist = dist;
                                                                closest_point = p2;
                                                                p_closest_rail = pRailNode;
                                                                found = true;
                                                            }
                                                        }
                                                    }
                                                } // end if (bb_test && Mth::LineLineIntersect(movement, segment, &p1, &p2, &f1, &f2))
                                            } // end if (Rail_ValidInEditor(pRailNode->m_pos,pRailNode->m_pNextLink->m_pos))
                                        } // end whole big bounding box test
                } // end if (pRailNode->m_pNextLink)
                else if (!pRailNode->pPrevLink && !pRailNode->pNextLink)
                {
                    // special logic for a single node rail
                    if (!(bb_min.x > pRailNode->vBBMax.x) &&
                        (!(bb_max.x < pRailNode->vBBMin.x)) &&
                        (!(bb_min.z > pRailNode->vBBMax.z)) &&
                        (!(bb_max.z < pRailNode->vBBMin.z)) &&
                        (!(bb_min.y > pRailNode->vBBMax.y)) &&
                        (!(bb_max.y) < pRailNode->vBBMin.y))
                    {
                        // calculate the distance from the movement to the rail point 
                        D3DXVECTOR3 offset = *(D3DXVECTOR3*)&pRailNode->vPos - pos1;
                        D3DXVECTOR3 movement_direction = ((static_cast<Vertex>(pos2 - pos1)).Normalize());
                        offset -= D3DXVec3Dot(&offset, &movement_direction) * movement_direction;
                        float distance = (*(Vertex*)&(offset)).Length();

                        if (distance > snap_dist) continue;

                        // single node rails count as twice the distance when looking for the closest rail
                        if (closest_dist < 2.0f * distance) continue;

                        closest_dist = 2.0f * distance;
                        closest_point = *(D3DXVECTOR3*)&pRailNode->vPos;
                        p_closest_rail = pRailNode;
                        found = true;
                    }
                }
            } // end if (active && etc)

            //pRailNode = pRailNode->m_pNext;
        }

        if (found)
        {
            // note, the line from pos1 to closest_point will not reflect the line segment found above
            // as the line segment will actually start somewhere between pos1 and pos2, and not always on pos1
    		/*if ( closest_dist > 40.0f)
    		{
    			found = false;
    		}
    		else*/
            {
                _printf("%X\n", &Game::skater->mp_rail_node);
                *p_point = closest_point;
                *pp_rail_node = p_closest_rail;
            }
        }
        return found;
    }


    static void FixRailLinks()
    {
        if (numRailNodes)
        {
            EndOfRail = (DWORD)&temp_nodes[current_node];
            RailNode** pp_railnodes = (RailNode**)mallocx(numNodes * sizeof(RailNode*));
            for (int i = 0; i < numNodes; i++)
            {
                pp_railnodes[i] = NULL;
            }

            RailNode* p_node;// = *first_node;
            int last = current_node - 1;
            *first_node = &temp_nodes[0];
            /*char test_msg[50];
            sprintf(test_msg, "%X\n", &Game::skater->mp_rail_node);
            MessageBox(0,test_msg,test_msg,0);
            MessageBox(0, 0, 0, 0);*/
            EndOfRail = (DWORD)&temp_nodes[current_node];

            for (int node = 0; node < current_node; node++)
            {
                p_node = &temp_nodes[node];
                /*if (node != last)
                    p_node->pNext = p_node + 1;
                else
                    p_node->pNext = NULL;*/
                pp_railnodes[p_node->node] = p_node;
                //p_node = p_node->pNext;
            }

            //p_node = *first_node;
            for (int node = 0; node < current_node; node++)
            {
                p_node = &temp_nodes[node];

                for (int i = 0; i < MAX_RAIL_LINKS; i++)
                {
                    signed short link = temp_links[node].m_link[i];
                    if (temp_links[node].m_link[i] != -1)
                    {

                        RailNode* to = pp_railnodes[link];

                        if (to)
                        {
                            NewLink(p_node, to);
                            p_node->link = p_node->pNextLink->node;
                        }
                        else
                            MessageBox(0, "Bad link?", "", 0);
                    }
                }
                //p_node = p_node->pNext;
            }

            freex(pp_railnodes);
            freex(temp_links);

            pointy_rails.clear();
            PointyObjects.clear();
            mp_nodes = (RailNode*)mallocx((current_node + pointy_rails.size())* sizeof(RailNode));
            DWORD temp_counter = 0;

            vector<RailNode*> alreadyAdded;

            for (DWORD i = 0; i < current_node; i++)
            {
                bool found = false;
                for (DWORD j = 0; j < alreadyAdded.size(); j++)
                {
                    if (alreadyAdded[j] == &temp_nodes[i])
                        found = true;
                }
                if (found)
                    continue;

                RailNode* pRail = &temp_nodes[i];
                mp_nodes[temp_counter] = *pRail;
                temp_counter++;
                alreadyAdded.push_back(pRail);
                RailNode* pPrev = pRail->pPrevLink;
                RailNode* p_loop = pRail;
                bool loop_advance = false;
                while (pPrev && pPrev != pRail && pPrev != p_loop)
                {
                    found = false;
                    for (DWORD j = 0; j < alreadyAdded.size(); j++)
                    {
                        if (alreadyAdded[j] == pPrev)
                            found = true;
                    }
                    if (!found)
                    {
                        mp_nodes[temp_counter++] = *pPrev;
                        alreadyAdded.push_back(pPrev);
                    }
                    pPrev = pPrev->pPrevLink;
                    if (!loop_advance)
                        p_loop = p_loop->pPrevLink;
                    loop_advance = !loop_advance;
                }

                RailNode* pNext = pRail->pNextLink;
                p_loop = pRail;
                loop_advance = false;
                while (pNext && pNext != pRail && pNext != p_loop)
                {
                    found = false;
                    for (DWORD j = 0; j < alreadyAdded.size(); j++)
                    {
                        if (alreadyAdded[j] == pNext)
                            found = true;
                    }
                    if (!found)
                    {
                        mp_nodes[temp_counter++] = *pNext;
                        alreadyAdded.push_back(pNext);
                    }
                    pNext = pNext->pNextLink;
                    if (!loop_advance)
                        p_loop = p_loop->pNextLink;
                    loop_advance = !loop_advance;
                }
                
            }
            if (temp_counter != current_node)
                MessageBox(0, "NOT GOOD", "", 0);

            for (DWORD i = 0; i < temp_counter; i++)
            {
                RailNode* pNext = mp_nodes[i].pNextLink;
                if (pNext)
                {
                    DWORD node = pNext->node;
                    for (DWORD j = 0; j < temp_counter; j++)
                    {
                        if (mp_nodes[j].node == node)
                        {
                            mp_nodes[i].pNextLink = &mp_nodes[j];
                            break;
                        }
                    }
                }
                RailNode* pPrev = mp_nodes[i].pPrevLink;
                if (pPrev)
                {
                    DWORD node = pPrev->node;
                    for (DWORD j = 0; j < temp_counter; j++)
                    {
                        if (mp_nodes[j].node == node)
                        {
                            mp_nodes[i].pPrevLink = &mp_nodes[j];
                            break;
                        }
                    }
                }
            }
            for (DWORD i = 0; i < pointy_rails.size(); i++)
            {
                bool found = false;
                for (DWORD j = 0; j < temp_counter; j++)
                {
                    D3DXVECTOR3 dist = *(D3DXVECTOR3*)&mp_nodes[j].vPos - *(D3DXVECTOR3*)&pointy_rails[i].vPos;
                    if (D3DXVec3Length(&dist) < 5.0f)
                    {
                        CArray* NodeArray = Node::GetNodeArray();
                        CStruct* pStruct = NodeArray->GetStructure(pointy_rails[i].link);
                        DWORD name = 0;
                        if (pStruct->GetChecksum(Checksums::Name, &name) && GameState::GotSuperSectors)
                        {
                            SuperSector* sector = SuperSector::GetSuperSector(name);
                            for (DWORD k = 0; k < PointyObjects.size(); k++)
                            {
                                if (PointyObjects[k] == sector)
                                {
                                    _printf("Removing auto-gen rail %s\n", FindChecksumName(name));
                                    PointyObjects.erase(PointyObjects.begin() + k);
                                    break;
                                }
                            }
                        }
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    mp_nodes[temp_counter] = pointy_rails[i];
                    temp_counter++;
                    current_node++;
                }
            }

            for (DWORD i = 0; i < pointy_rails.size(); i++)
            {
                CArray* NodeArray = Node::GetNodeArray();
                CStruct* pStruct = NodeArray->GetStructure(pointy_rails[i].link);
                DWORD name = 0;
                if (pStruct->GetChecksum(Checksums::Name, &name) && GameState::GotSuperSectors)
                {
                    SuperSector* sector = SuperSector::GetSuperSector(name);
                    if (sector)
                    {
                        /*bbox_rails.push_back(ColouredVertex(sector->bboxMin.x, sector->bboxMin.y, sector->bboxMin.z));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x, sector->bboxMin.y, sector->bboxMin.z));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x, sector->bboxMax.y, sector->bboxMin.z));

                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x, sector->bboxMax.y, sector->bboxMin.z));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x, sector->bboxMin.y, sector->bboxMin.z));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x, sector->bboxMax.y, sector->bboxMin.z));

                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x, sector->bboxMin.y, sector->bboxMin.z));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x, sector->bboxMax.y, sector->bboxMin.z));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x, sector->bboxMin.y, sector->bboxMin.z));

                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x, sector->bboxMin.y, sector->bboxMin.z));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x, sector->bboxMax.y, sector->bboxMin.z));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x, sector->bboxMin.y, sector->bboxMax.z));*/


                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x-50.0f, sector->bboxMin.y-50.0f, sector->bboxMin.z-50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x+50.0f, sector->bboxMin.y-50.0f, sector->bboxMin.z-50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x+50.0f, sector->bboxMin.y-50.0f, sector->bboxMin.z-50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x+50.0f, sector->bboxMax.y+50.0f, sector->bboxMin.z-50.0f));

                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x+50.0f, sector->bboxMax.y+50.0f, sector->bboxMin.z-50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x-50.0f, sector->bboxMax.y+50.0f, sector->bboxMin.z-50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x-50.0f, sector->bboxMax.y+50.0f, sector->bboxMin.z-50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x-50.0f, sector->bboxMin.y-50.0f, sector->bboxMin.z-50.0f));

                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x-50.0f, sector->bboxMin.y-50.0f, sector->bboxMax.z-50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x+50.0f, sector->bboxMin.y-50.0f, sector->bboxMax.z+50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x+50.0f, sector->bboxMin.y-50.0f, sector->bboxMax.z+50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x, sector->bboxMax.y, sector->bboxMax.z));

                        bbox_rails.push_back(ColouredVertex(sector->bboxMax.x+50.0f, sector->bboxMax.y+50.0f, sector->bboxMax.z+50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x-50.0f, sector->bboxMax.y+50.0f, sector->bboxMax.z+50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x-50.f, sector->bboxMax.y+50.0f, sector->bboxMax.z+50.0f));
                        bbox_rails.push_back(ColouredVertex(sector->bboxMin.x-50.f, sector->bboxMin.y-50.0f, sector->bboxMax.z+50.0f));
                    }
                }
            }

            pointy_rails.clear();
            freex(temp_nodes);
            temp_nodes = NULL;
            //ExecuteQBScript("LoadTerrain");
        }
    }

    static void NewLink(RailNode* p_from, RailNode* p_to)
    {
        // one of our links links to this node
        // so we need to establish next and prev links
        // or if they are established, then possibly override them			
        if (!p_from->pNextLink)
        {
            // this is the first time we are linking to something
            p_from->pNextLink = p_to;
            Rail_ComputeBB(p_from->vPos, p_to->vPos, p_from->vBBMin, p_from->vBBMax);
        }
        else
        {

            // already linked to something, so only override it
            // if the new node is not DEFAULT_LINE
            if (p_to->GetFlag(RailNode::Flags::DEFAULT_LINE))
            {
                p_from->pNextLink = p_to;
                Rail_ComputeBB(p_from->vPos, p_to->vPos, p_from->vBBMin, p_from->vBBMax);
            }
            else
            {
                /*if (!p_from->pNextLink->GetFlag(RailNode::Flags::DEFAULT_LINE))
                {
                    p_from->pNextLink = p_to;
                    Rail_ComputeBB(p_from->vPos, p_to->vPos, p_from->vBBMin, p_from->vBBMax);
                }
                // The new link is not DEFAULT_LINE
                // if the old one was, then leave it
                // if neither is, then leave it, and maybe print a warning?
                MessageBox(0, "RailManager: ERROR", "Multiple links but no DefaultLine", 0);*/
                if (!p_from->pNextLink->GetFlag(RailNode::Flags::DEFAULT_LINE))
                    _printf("RailManager: ERROR", "Multiple links but no DefaultLine\n");
            }
        }

        // Now handle the PREV link back from p_to to p_from

        if (!p_to->pPrevLink)
        {
            p_to->pPrevLink = p_from;
        }
        else
        {

            // something already linked to p_from via prev from p_to
            if (p_from->GetFlag(RailNode::Flags::DEFAULT_LINE))
            {
                p_to->pPrevLink = p_from;
            }
        }
    }

    static const Vertex& const __restrict GetPos(const RailNode* pNode)
    {
        return *(Vertex*)&pNode->vPos;
    }

    static void SetActive(int node, int active, bool wholeRail)
    {

        //	CRailNode *pRailNode = mp_first_node;
        //	while (pRailNode)
        for (int check_node = 0; check_node < current_node; check_node++)
        {
            RailNode* __restrict pRailNode = temp_nodes ? &temp_nodes[check_node] : &mp_nodes[check_node];

            if (pRailNode->node == node)
            {
                if (wholeRail)
                {
                    while (pRailNode)
                    {
                        pRailNode->SetActive(active);
                        pRailNode = pRailNode->pNextLink;
                    }
                    return;
                }
                pRailNode->SetActive(active);
                return;
            }
            //		pRailNode = pRailNode->m_pNext;
        }
    }

    // returns a positive number that is the amount of an axis
// required to totally encompass the 1D line segments a-b and c-d
    static inline float span_4(float a, float b, float c, float d)
    {
        float min = a;
        if (b < min) min = b;
        if (c < min) min = c;
        if (d < min) min = d;
        float max = a;
        if (b > max) max = b;
        if (c > max) max = c;
        if (d > max) max = d;
        return max - min;
    }

    // returns the amount two 1D line segments overlap
// works by adding together the length of both lines
// and then subtracting the "span" of the the min and max points of the lines															 
    static inline float overlap(float a, float b, float c, float d)
    {
        return  fabsf(b - a) + fabsf(d - c) - span_4(a, b, c, d);
    }

    static inline bool nearly_the_same(float a, float b, float c, float d, float nearly = 1.0f)
    {
        return (fabsf(a - b) < nearly
            && fabsf(b - c) < nearly
            && fabsf(c - d) < nearly);
    }

    static void	RemoveOverlapping()
    {
        if (!ParkEditor::Instance()->UsingCustomPark())
            return;
        _printf("Starting overlapping rail removal\n");

        int	removed = 0;
        //	CRailNode *pRailNode = mp_first_node;
        //	while (pRailNode)
        //	{
        for (int node = 0; node < current_node; node++)
        {
            RailNode* pRailNode = &mp_nodes[node];
            if (pRailNode->pNextLink)	   	// it's a segment
            {

                Vertex start = *(Vertex*)&pRailNode->vPos;
                Vertex end = *(Vertex*)&pRailNode->pNextLink->vPos;

                Vertex dir = end - start;
                dir.Normalize();

                // we expand the bounding box, as they might not really be that close			
                Vertex bb_min = *(Vertex*)&pRailNode->vBBMin;
                bb_min[X] -= 16.0f;
                bb_min[Y] -= 16.0f;
                bb_min[Z] -= 16.0f;
                Vertex bb_max = *(Vertex*)&pRailNode->vBBMax;
                bb_max[X] += 16.0f;
                bb_max[Y] += 16.0f;
                bb_max[Z] += 16.0f;


                int check_node = node + 1;
                //			CRailNode *pCheckNode = pRailNode->m_pNext;
                while (check_node < current_node && pRailNode->IsActive())
                {
                    RailNode* pCheckNode = &mp_nodes[check_node];
                    if (pCheckNode->pNextLink)	   	// it's a segment
                    {
                        // first check to see if bounding boxes overlap

                        if (!(pCheckNode->vBBMin.x > bb_max.x))
                            if (!(pCheckNode->vBBMax.x < bb_min.x))
                                if (!(pCheckNode->vBBMin.z > bb_max.z))
                                    if (!(pCheckNode->vBBMax.z < bb_min.z))
                                        if (!(pCheckNode->vBBMin.y > bb_max.y))
                                            if (!(pCheckNode->vBBMax.y < bb_min.y))
                                            {

                                                // bounding boxes overlap 						
                                                // check to see if rails are roughly parallel
                                                Vertex check_start = *(Vertex*)&pCheckNode->vPos;
                                                Vertex check_end = *(Vertex*)&pCheckNode->pNextLink->vPos;
                                                Vertex check_dir = check_end - check_start;

                                                check_dir.Normalize();
                                                float dot = D3DXVec3Dot(&dir, &check_dir);
                                                //						printf ("Bounding Box Overlap, dot = %f\n",dot);

                                                if (dot < -0.99f || dot > 0.99f)
                                                {
                                                    // lines are roughly parallel
                                                    // so check if they overlap significantly in at least one axis
                                                    // and/or are very close in one other axis
                                                    // we only check X and Z
                                                    // as we don't have any vertical rails in the park
                                                    // editor
                                                    // (note, I'm prematurely optimizing here...)

                                                    int overlaps = 0;
                                                    int close = 0;

                                                    // now check the distance between the lines
                                                    // which is the components of start -> check_start
                                                    // that is perpendicular to start -> end
                                                    const float significant = 6.0f;	// six inches is significant, I feel...
                                                    Vertex	s_cs = check_start - start;
                                                    Vertex	s_e = end - start;
                                                    s_e.Normalize();
                                                    Vertex perp = s_cs;
                                                    perp.ProjectToPlane(s_e);
                                                    if (perp.Length() < significant)
                                                    {

                                                        //								printf ("(%.2f,%.2f),(%.2f,%.2f),(%.2f,%.2f),(%.2f,%.2f)\n",
                                                        //								start[X],start[Z],end[X],end[Z],
                                                        //								check_start[X],check_start[Z],check_end[X],check_end[Z]);

                                                        if (overlap(start[X], end[X], check_start[X], check_end[X]) > significant)
                                                            overlaps++;
                                                        if (overlap(start[Z], end[Z], check_start[Z], check_end[Z]) > significant)
                                                            overlaps++;
                                                        if (nearly_the_same(start[X], end[X], check_start[X], check_end[X], 6.0f))
                                                            close++;
                                                        if (nearly_the_same(start[Z], end[Z], check_start[Z], check_end[Z], 6.0f))
                                                            close++;
                                                        //								printf ("dot close, overlaps = %d, close = %d\n",overlaps,close);
                                                        if (overlaps + close >= 2)
                                                        {
                                                            // it's a duplicate
                        //									printf("Removing duplicate rail !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

                                                            // add a magenta line for rails that have been removed
#ifdef		DEBUG_EDITOR_RAILS
                                                            pRailNode->m_pos += Mth::Vector(0.0f, 2.0f, 0.0f);
                                                            pRailNode->m_pNextLink->m_pos += Mth::Vector(0.0f, 2.0f, 0.0f);
                                                            Gfx::AddDebugLine(pRailNode->m_pos, pRailNode->m_pNextLink->m_pos, MAKE_RGB(255, 0, 255), MAKE_RGB(255, 0, 255));
                                                            pRailNode->m_pos -= Mth::Vector(0.0f, 2.0f, 0.0f);
                                                            pRailNode->m_pNextLink->m_pos -= Mth::Vector(0.0f, 2.0f, 0.0f);
#endif

                                                            removed++;
                                                            // Remove the one that is lower
                                                            if (pRailNode->vPos[Y] < pCheckNode->vPos[Y])
                                                            {
                                                                pRailNode->SetActive(false);
                                                            }
                                                            else
                                                            {
                                                                pCheckNode->SetActive(false);
                                                            }
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                    }
                    check_node++;
                    //				pCheckNode = pCheckNode->m_pNext;
                }
            }
            //		pRailNode = pRailNode->m_pNext;
        }
        _printf("Done overlapping rail removal, removed %d\n", removed);

        for (int node = 0; node < current_node; node++)
        {
            RailNode* pRailNode = &mp_nodes[node];

            if (pRailNode->IsActive())
            {
                RailNode* pEnd = pRailNode->pNextLink;

                if (pEnd && pEnd->IsActive())
                {
                    if (!Rail_ValidInEditor(*(Vertex*)&pRailNode->vPos, *(Vertex*)&pEnd->vPos))
                    {
                        RailNode* pNext = pEnd->pNextLink;
                        if (!pNext || !Rail_ValidInEditor(*(Vertex*)&pEnd->vPos, *(Vertex*)&pNext->vPos))
                            pRailNode->SetActive(false);
                    }
                    else
                    {
                        /*RailNode* pPrev = pRailNode->pPrevLink;
                        if (pPrev)
                        {
                            if (!Rail_ValidInEditor(*(Vertex*)&pRailNode->vPos, *(Vertex*)&pPrev->vPos))
                            {
                                pRailNode->SetActive(false);
                            }
                        }*/
                    }
                }
            }
            else
            {
                /*RailNode* pPrev = pRailNode->pPrevLink;
                if (pPrev)
                {
                    if (!Rail_ValidInEditor(*(Vertex*)&pRailNode->vPos, *(Vertex*)&pPrev->vPos))
                    {
                        pRailNode->SetActive(false);
                    }
                }*/
            }
        }
    }

    // use in-game collision checks to determine if a rail is valid
    static bool  Rail_ValidInEditor(const Vertex& Start, const Vertex& End)
    {

        // MAYBE TODO:  a rail should only need disqualifying if it is along the edge of a cell
        // (or even more accurately, a meta-piece
        // so could maybe check for that before we try to disqualify it. 


        if (!ParkEditor::Instance()->UsingCustomPark()) 		// is it a custom park???
        {
#ifdef		DEBUG_EDITOR_RAILS
            printf("not in editor\n");
#endif
            // if not a custom park, then everything is valid.
            return true;
    }

        Game::skater->Store();

        // Find a point "Mid", which is 1/4 of the way along the rail   
        Vertex	Mid = (Start + End) / 2.0f;			// Half way along
        Mid = Start + ((Mid - Start) / 2.0f);		    // quarter of the way along	
        Mid[Y] += 6.0f;									// raised up a bit

    // Find a vector "Side" which is horizontal, perpendicular to the rail, and 4 inches long	
        Vertex Side = End - Start;	   			// vector along the rail
        Side[Y] = 0.0f;								// horizontal
        Side.Normalize(7.0f);						// this is half the width of the thickest rail.  Was 4.0, changed for Tokyo mega funbox
        float temp = Side[X];			 				// make perpendicular to rail
        Side[X] = Side[Z];
        Side[Z] = -temp;



        // if collision above me, left to right, then invalid
        Game::skater->SetRay(Mid + Side, Mid - Side);
        if (Game::skater->CollisionCheck())
        {
#ifdef		DEBUG_EDITOR_RAILS
            printf("l-r above collision, invalid\n");
            feeler.DebugLine(255, 0, 0);
#endif
            return false;
        }
        else
        {
#ifdef		DEBUG_EDITOR_RAILS
            feeler.DebugLine();
#endif
        }

        // if collision above me, right to left, then invalid 	
        Game::skater->FlipDirection();
        if (Game::skater->CollisionCheck())
        {
#ifdef		DEBUG_EDITOR_RAILS
            printf("r-l above collision, invalid\n");
            feeler.DebugLine(255, 0, 0);
#endif
            Game::skater->Restore();
            return false;
        }
        else
        {
#ifdef		DEBUG_EDITOR_RAILS
            feeler.DebugLine();
#endif
        }

#if 0

        // Get a vector "Down", which is a line straight down 7 inches
        Vertex	DeepBelow(0.0f, -12.0f, 0.0f);	 		// six inches below the rail

        float	left_height = 0.0f;
        float 	right_height = 0.0f;

        // find reltive height of slope (to rail) on left side
        Game::skater->SetRay(Mid + Side, Mid + Side + DeepBelow);
        if (Game::skater->CollisionCheck())
        {
#ifdef		DEBUG_EDITOR_RAILS
            feeler.DebugLine(0, 255, 0);	   	// green = possible bad one side
#endif
            left_height = *Game::skater->GetHitPoint()[Y] - (Mid[Y] - 6.0f);
        }
        else
        {
            return true;
        }

        Game::skater->SetRay(Mid - Side, Mid - Side + DeepBelow);
        if (Game::skater->CollisionCheck())
        {
#ifdef		DEBUG_EDITOR_RAILS
            feeler.DebugLine(0, 255, 0);	   	// green = possible bad one side
#endif
            right_height = *Game::skater->GetHitPoint()[Y] - (Mid[Y] - 6.0f);
        }
        else
        {
            return true;
        }


        //	printf("Left = %f, Right = %f\n",left_height, right_height);

        if (left_height > -1.0f && right_height > -1.0f)
        {
            // both faces tilt upwards, or are roughtly horizontal
            // so return false
            return false;
        }

        // Make it so left is higher than right for subsequent tests	
        if (left_height < right_height)
        {
            float t = left_height;
            left_height = right_height;
            right_height = t;
        }

        // check for steep left side
        if (left_height > 1.0f)
        {
            // sloped down right side, so return
            if (right_height > -2.0f)
            {
                //			printf ("Sloped, false\n");
                return false;
            }
        }


#else
        // Get a vector "Down", which is a line straight down 7 inches
        Vertex	DownBelow(0.0f, -7.0f, 0.0f);	 		// inch below the rail
        Vertex	DownAbove(0.0f, -4.0f, 0.0f);			// 2 inch above the rail
        // we do fource collision checks, two on each side of the rail
        // one to a height that goes below the rail
        // and the other that goes to a height above it
        // if two or more of these return a collision
        // then the rail is invalid
        // the majority of bad rails will be eliminated with two checks
        int cols = 0;
        Game::skater->SetRay(Mid + Side, Mid + Side + DownBelow);
        if (Game::skater->CollisionCheck())
        {
            cols++;
#ifdef		DEBUG_EDITOR_RAILS
            feeler.DebugLine(0, 255, 0);	   	// green = possible bad one side
            printf("first side collision (greeen), checking other side.....\n");
#endif
        }

        Game::skater->SetRay(Mid - Side, Mid - Side + DownBelow);
        if (Game::skater->CollisionCheck())
        {
            cols++;
#ifdef		DEBUG_EDITOR_RAILS
            feeler.DebugLine(0, 255, 0);	   	// green = possible bad one side
            printf("second side collision (greeen), checking other side.....\n");
#endif
            if (cols > 1)
            {
                Game::skater->Restore();
                return false;
            }
        }

        Game::skater->SetRay(Mid + Side, Mid + Side + DownAbove);
        if (Game::skater->CollisionCheck())
        {
            cols++;
#ifdef		DEBUG_EDITOR_RAILS
            feeler.DebugLine(0, 255, 0);	   	// green = possible bad one side
            printf("third side collision (greeen), checking other side.....\n");
#endif
            if (cols > 1)
            {
                Game::skater->Restore();
                return false;
            }
        }

        Game::skater->SetRay(Mid - Side, Mid - Side + DownAbove);
        if (Game::skater->CollisionCheck())
        {
            cols++;
#ifdef		DEBUG_EDITOR_RAILS
            feeler.DebugLine(0, 255, 0);	   	// green = possible bad one side
            printf("forth side collision (greeen)\n");
#endif
            if (cols > 1)
            {
                Game::skater->Restore();
                return false;
            }
        }
#endif			

        // Not found two collisions
        // so we can return true, indicating this rail is valid in the park editor
        //	printf ("everything ok, valid\n");
        Game::skater->Restore();
        return true;
    }

    void AddRailNode(DWORD index, CStruct* pNodeStruct)
    {
        if (current_node == MAX_RAIL_NODES)
        {
            MessageBox(0, "Now it's fuckedup...", "RailNodes....", 0);
            return;
        }

        if (!SkateMod::ShouldBeAbsentNode(pNodeStruct))
        {
            RailNode* pRailNode = &temp_nodes[current_node];
            /*pRailNode->pNext = *first_node;
            *first_node = pRailNode;*/
            RailLinks* pLinkNode = &temp_links[current_node++];
            if (current_node > numRailNodes)
                MessageBox(0, "Too many rails?", "", 0);

            DWORD class_checksum = 0;
            pNodeStruct->GetChecksum(Checksums::Class, &class_checksum);
            bool climbing = class_checksum == Checksum("ClimbingNode");

            DWORD type_checksum = 0;
            pNodeStruct->GetChecksum(Checksums::Type, &type_checksum);

            //pRailNode->pNext = NULL;
            pRailNode->pNextLink = NULL;
            pRailNode->pPrevLink = NULL;

            pRailNode->terrain = 0;
            if (!pNodeStruct->GetEnum(Checksums::TerrainType, &pRailNode->terrain, false))
                _printf("Terrain not found\n");

            for (int i = 0; i < MAX_RAIL_LINKS; i++)
            {
                pLinkNode->m_link[i] = -1;
            }

            pRailNode->node = index;
            pRailNode->link = 0xFFFF;
            pRailNode->flags = 0;
            pRailNode->SetActive(pNodeStruct->ContainsFlag(Checksums::CreatedAtStart));

            Node::GetPosition(pNodeStruct, &pRailNode->vPos);

            if (pNodeStruct->ContainsFlag(Checksum("LipOverride")))
            {
                pRailNode->SetFlag(RailNode::Flags::LIP_OVERRIDE);
            }


            if (pNodeStruct->ContainsFlag(Checksum("DefaultLine")))
            {
                pRailNode->SetFlag(RailNode::Flags::DEFAULT_LINE);
            }

            if (climbing)
            {
                pRailNode->SetFlag(RailNode::Flags::ONLY_CLIMBING);
                pRailNode->SetFlag(RailNode::Flags::LADDER, type_checksum == Checksum("Ladder"));
            }


            const CArray* links;
            if (pNodeStruct->GetArray("links", &links))
            {
                int numLinks = links->GetNumItems();
                if (numLinks > MAX_RAIL_LINKS)
                {
                    MessageBox(0, "TOO MANY LINKS", "", 0);
                    numLinks = MAX_RAIL_LINKS;
                }
                for (auto i = 0; i < numLinks; i++)
                {
                    signed short link = (*links)[i];
                    if (link >= numNodes || link < 0)
                        MessageBox(0, "BAD LINNK", "", 0);
                    else
                        pLinkNode->m_link[i] = link;
                }
            }

            Rail_ComputeBB(pRailNode->vPos, pRailNode->vPos, pRailNode->vBBMin, pRailNode->vBBMax);

        }
    }

};

void Skater::maybe_trip_rail_trigger(DWORD type)
{
    // given that m_rail_node is valid, then trigger any script associated with this rail node
    // will search backwards for the first rail that has a trigger script, and then execute that.

    if (!mp_rail_node) return;

    const RailNode* pStartOfRail = mp_rail_node;
    const RailNode* pRail = mp_rail_node;

    // no node array in rail manager indicates auto generated rails, so just return
    CArray* pNodeArray = Node::GetNodeArray();
    if (!pNodeArray) return;

    CStruct* pNode = pNodeArray->GetStructure(mp_rail_node->GetNode());

    // find a rail node that has a "TriggerScript" in it
    DWORD trigger = 0;
    if (pNode->GetChecksum(Checksums::TriggerScript, &trigger))
    {
        // we got it 
    }
    else
    {
        // did not get it, so scoot backwards until we detect a loop or we find one
        const RailNode* p_loop_detect = pRail;	 	// start loop detect at the start
        pRail = pRail->GetPrevLink(); 				// and the first node we check is the next one
        bool loop_advance = false;
        while (pRail && pRail != pStartOfRail && pRail != p_loop_detect)
        {
            pNode = pNodeArray->GetStructure(pRail->GetNode());
            if (pNode->GetChecksum(Checksums::TriggerScript, &trigger)) break;

            pRail = pRail->GetPrevLink();
            // The p_loop_detect pointer goes backwards at half speed, so if there is a loop
            // then pRail is guarenteed to eventually catch up with p_loop_detect
            if (loop_advance)
            {
                p_loop_detect = p_loop_detect->GetPrevLink();
            }
            loop_advance = !loop_advance;
        }
    }

    if (trigger)
    {
        _printf("%s -> %s\n", FindChecksumName(pNode->GetName()), FindChecksumName(trigger));

        DWORD node_name;
        pNode->GetChecksum(Checksums::Name, &node_name, QScript::ASSERT);

        //Think this is needed for graf?
        SetName(node_name);

        //Actually spawn the TriggerScript
        TripTrigger(type, trigger, pRail->GetNode(), pNode);
    }
    /*else
    {
        pNode->GetChecksum(Checksums::Name, &m_last_rail_node_name, QScript::ASSERT);
    }*/
}
__declspec(naked) void maybe_skate_off_rail()
{
    static const DWORD jmpBack = 0x004A5ABC;
    static bool last_segment;
    static RailNode* pFrom;
    static RailNode* pOnto;
    static DWORD extra_dist;
    _asm mov eax, [esp + 0x28];
    _asm mov pFrom, eax;
    _asm mov eax, [esp + 0x68];
    _asm mov pOnto, eax;
    _asm mov al, [esp + 0x13];
    _asm mov last_segment, al;
    _asm mov extra_dist, esp
    extra_dist += 0x2C;
    Game::skater->MaybeSkateOffRail(last_segment, *(Vertex*)&extra_dist, pFrom, pOnto);
    _asm jmp[jmpBack];
}

bool skated_off_rail = false;

__inline void Skater::MaybeSkateOffRail(bool last_segment, Vertex& extra_dist, RailNode* pFrom, RailNode* pOnto)
{
    //_printf("MaybeSkateOffRail last_segment %X extra_dist%X\n", &last_segment, &extra_dist);
    // recalculate start, end, dir, as we might be on a new segment
    const RailNode* pStart = mp_rail_node;
    const RailNode* pEnd = pStart->GetNextLink();

    Vertex	dir = RailManager::GetPos(pEnd) - RailManager::GetPos(pStart);
    dir.Normalize();

    // sign also may have changed, now that we are auto-linking rail segments

    // sign is which way we are going along the rail
    float sign = Sgn(D3DXVec3Dot(&dir, GetVelocity()));

    m_rail_time = GetCurrentTime();

    ((Vertex*)GetVelocity())->RotateToNormal(dir);
    random = 0;
    *(Vertex*)GetVelocity() *= sign;						   						// sign won't be on a new segment
    float facing_sign = bRail_Backwards ? -sign : sign;

    // z is forward
    Vertex target_forward = dir * facing_sign;

    //Need to calculate matrices in order z, x , y

    //First the lerp matrix
    //float temp = GetLerpMatrix()[Y][X] * GetLerpMatrix[Z][Y];

    /*
    *(Vertex*)&GetLerpMatrix()[Z] = Lerp(*(Vertex*)&GetLerpMatrix()[Z], target_forward, 0.3f);
    (*(Vertex*)&GetLerpMatrix()[Z]).Normalize();

#if 1 // old code
    GetLerpMatrix()[Y].Set(0.0f, 1.0f, 0.0f);
    *(Vertex*)&GetLerpMatrix()[X] = CrossProduct(
        (Vertex*)&GetLerpMatrix()[Y],
        (Vertex*)&GetLerpMatrix()[Z]
    );
#else
    (*(Vector*)&GetLerpMatrix()[X]).Set(
        GetLerpMatrix()[Z][Z],
        0.0f,
        -GetLerpMatrix()[Z][X],
        0.0f
    );
#endif
    (* (Vertex*)&GetLerpMatrix()[X]).Normalize();

    *(Vertex*)&GetLerpMatrix()[Y] = CrossProduct(
        (Vertex*)&GetLerpMatrix()[Z],
        (Vertex*)&GetLerpMatrix()[X]
    );
    GetLerpMatrix()[Y][W] = 1.0f;
    (*(Vertex*)&GetLerpMatrix()[Y]).Normalize();

    // adjust our Z value towards the new value		
    *(Vertex*)&GetMatrix().m[Z] = target_forward;
    GetMatrix().m[Z][W] = 0.0f;
    (*(Vertex*)&GetMatrix().m[Z]).Normalize();

#if 1 // old code
    (* (Vector*)&GetMatrix().m[Y]).Set(0.0f, 1.0f, 0.0f);
    *(Vertex*)&GetMatrix().m[X] = CrossProduct((Vertex*)&GetMatrix().m[Y], (Vertex*)&GetMatrix().m[Z]);
#else
    (* (Vector*)&GetMatrix().m[X]).Set(
        GetMatrix().m[Z][Z],
        0.0f,
        -GetMatrix().m[Z][X],
        0.0f
    );
#endif
    (*(Vertex*)&GetMatrix()[X]).Normalize();

    *(Vertex*)&GetMatrix()[Y] = CrossProduct((Vertex*)&GetMatrix().m[Z], (Vertex*)&GetMatrix().m[X]);
    (*(Vertex*)&GetMatrix()[Y]).Normalize();
    GetMatrix().m[Y][W] = 1.0f;
    */

    
    *(Vertex*)&GetLerpMatrix()[Z] = Lerp(*(Vertex*)&GetLerpMatrix()[Z], target_forward, 0.3f);
    (*(Vertex*)&GetLerpMatrix()[Z]).Normalize();

    GetLerpMatrix()[X].Set(GetLerpMatrix()[Z][Z], 0.0f, -GetLerpMatrix()[Z][X], 1.0f);
    (*(Vertex*)&GetLerpMatrix()[X]).Normalize();

    *(Vertex*)&GetLerpMatrix()[Y] = CrossProduct((Vertex*)&GetLerpMatrix()[Z], (Vertex*)&GetLerpMatrix()[X]);
    (*(Vertex*)&GetLerpMatrix()[Y]).Normalize();

    //Then the real matrix											 
    *(Vertex*)&GetMatrix().m[Z] = target_forward;
    (*(Vertex*)&GetMatrix().m[Z]).Normalize();

    (*(Matrix*)&GetMatrix())[X].Set(GetMatrix().m[Z][Z], 0.0f, -GetMatrix().m[Z][X], 1.0f);
    (*(Vertex*)&GetMatrix().m[X]).Normalize();

    *(Vertex*)&GetMatrix().m[Y] = CrossProduct((Vertex*)&GetMatrix().m[Z], (Vertex*)&GetMatrix().m[X]);
    (*(Vertex*)&GetMatrix().m[Y]).Normalize();
    
    
    // This is where we do the actual movement

    // if this makes us bump into the wall or the ground, then we should leave the rail

    Vertex old_pos = *GetPosition();
    *GetPosition() += *GetVelocity() * GetFrameLength();			// current movement
    Vertex temp;
    temp.x = extra_dist.x * target_forward.x;
    temp.y = extra_dist.y * target_forward.y;
    temp.z = extra_dist.z * target_forward.z;

    *(Vertex*)GetPosition() += temp;						// any extra dist from previous segment

    // Mick:  use "old_pos" to generate the direction
    // so it is guarenteed to be parallel to the rail
    // and m_pos might have been adjusted if we continued from
    // one rail to another (in-line) rail, like in the park editor
    Vertex movement = *GetPosition() - old_pos;
    Vertex direction = movement;
    direction.Normalize();

    bool always_check = false;
    if (!last_segment && ParkEditor::Instance()->UsingCustomPark())
    {
        // in the park editor we can have tight curves that end in walls, so we want to always do the forward check
        Vertex from = RailManager::GetPos(pFrom->GetNextLink()) - RailManager::GetPos(pFrom);
        Vertex onto = RailManager::GetPos(pOnto->GetNextLink()) - RailManager::GetPos(pOnto);
        from.Normalize();
        onto.Normalize();
        float delta = fabsf(from[Y] - onto[Y]);
        if (delta < 0.01f)
        {
            // lines have a sufficently close Y angle
            always_check = true;
        }
    }
    else if (last_segment && pFrom != mp_rail_node && pFrom->GetNextLink() != mp_rail_node)
    {
        Vertex from = RailManager::GetPos(pFrom->GetNextLink()) - RailManager::GetPos(pFrom);
        Vertex onto = RailManager::GetPos(mp_rail_node->GetNextLink()) - RailManager::GetPos(mp_rail_node);
        from.Normalize();
        onto.Normalize();
        float deltaZ = fabsf(from[Z] - onto[Z]);
        float deltaX = fabsf(from[X] - onto[X]);

        if (deltaZ >= 0.01f && deltaX >= 0.01f)
        {
            _printf("This is not normal\n");
            last_segment = false;
        }
    }

    // Only check for hitting a wall if we are on a segment of rail that has no more rail
    if (last_segment || always_check)
    {
        startcol = old_pos;
        endcol = *GetPosition();
        endcol += movement + (direction * 6.0f);

        // raise them up one inch, so we don't collide with the rail
        startcol += *(D3DXVECTOR3*)&GetMatrix().m[Y];
        endcol += *(D3DXVECTOR3*)&GetMatrix().m[Y];
        if (CollisionCheck())
        {
            // if in the park editor, then ignore collision with invisible surfaces 
            if (!ParkEditor::Instance()->UsingCustomPark() || normal.y < 0.5f)// || (!ParkEditor::Instance()->UsingCustomPark() && fabsf(D3DXVec3Length(&extra_dist)<0.5f)))
            {
                _printf("COllided offrail\n");
                //_printf("OffRail.... %f normal.y %f off %d\n", fabsf(D3DXVec3Length(&extra_dist)), normal.y, skated_off_rail);
                maybe_trip_rail_trigger(Node::TRIGGER_SKATE_OFF);
                // don't let him make this movement!!
                *GetPosition() = *GetOldPosition();
                *GetPosition()[Y] += 1.0f;

                // project velocity along the plane if we run into a wall
                ((Vertex*)GetVelocity())->ProjectToPlane(normal);

                SetState(AIR);			// knocked off rail, as something in front
                FlagException("OffRail");
            }
        }
    }
    skated_off_rail = false;
}

void Skater::SkateOffRail()
{
    skated_off_rail = true;
    // we have skated off a rail; either it was the end of a rail or we hit a sharp corner
    // we need to see if there is another rail in front of us that we can continue skating on

    bool really_off = true;

    D3DXVECTOR3	a;
    if (ParkEditor::Instance()->UsingCustomPark() && mp_rail_node)
    {
        // in the park editor, we use the last point on the rail we just took off from as the start of the line to use for looking for new rails
        // so we don't get rail segments earlier in the list
        float dist[3];

        D3DXVECTOR3 temp = (*(Vertex*)GetPosition() - RailManager::GetPos(mp_rail_node));
        dist[0] = D3DXVec3Length(&temp);
        if (mp_rail_node->GetNextLink())
        {
            temp = (*(Vertex*)GetPosition() - RailManager::GetPos(mp_rail_node->GetNextLink()));
            dist[1] = D3DXVec3Length(&temp);
        }
        else
            dist[1] = dist[0] + 1000.0f;

        if (mp_rail_node->GetPrevLink())
        {
            temp = (*(Vertex*)GetPosition() - RailManager::GetPos(mp_rail_node->GetPrevLink()));
                dist[2] = D3DXVec3Length(&temp);
        }
        else
            dist[2] = dist[0] + 1000.0f;
            float closest = dist[0];
            int dummy = 0;
            for (int i = 1; i < 3; i++)
            {
                if (dist[i] < closest)
                {
                    closest = dist[i];
                    dummy = i;
                }
            }
        switch (dummy)
        {
        case 0:
            a = RailManager::GetPos(mp_rail_node);
            break;
        case 1:
            a = RailManager::GetPos(mp_rail_node->GetNextLink());
            break;
        case 2:
            a = RailManager::GetPos(mp_rail_node->GetPrevLink());
            break;
        }

    }
    else
    {
        // go back a step
        a = *GetPosition() - *GetVelocity() * GetFrameLength();// *0.001f;
    }

    // use current pos, as we know that is off the end
    D3DXVECTOR3 b = *GetPosition();

    Vertex rail_pos;
    RailNode* pNode;


    // we need to tilt the line down, as the rail code currently fails to find a rail if the line we check is exactly parallel with it	
    a[Y] += 6.0f;
    b[Y] += 6.1f;

    float min_dot = cosf(DegToRad(Physics::Rail_Corner_Leave_Angle));

    // Note: we are now passing in Rail_Side() of the current rail and velocity
    // so we can see if we switch from a rail on a left facing ledge to a rail on a right facing ledge, and try to inhibit that type of transition
    // in favour of one that retains the same side
    if (RailManager::StickToRail(
        a,
        b,
        &rail_pos,
        &pNode,
        mp_rail_node,
        min_dot,
        mp_rail_node->Side(*GetVelocity())
    ))
    {
        // Mick, in park editor, we also disallow this if the rail is the next or the prev rail node from our current node
        if (mp_rail_node != pNode && pNode->GetNextLink() && (
            !ParkEditor::Instance()->UsingCustomPark()
            || (mp_rail_node->GetNextLink() != pNode
                && mp_rail_node->GetPrevLink() != pNode
                )))
        {
            const RailNode* pNewStart = pNode;
            const RailNode* pNewEnd = pNewStart->GetNextLink();

            // check to see if our new position is within the two points
            Vertex	to_start = RailManager::GetPos(pNewStart) - *GetPosition();
            Vertex to_end = RailManager::GetPos(pNewEnd) - *GetPosition();

            float mid_dot = D3DXVec3Dot(&to_start, &to_end);

            // In game, the point must actualy be in the line, so mid dot will be negative
            bool ok = mid_dot < 0.0f;

            // Park Editor specific rail joining			
            if (!ok && ParkEditor::Instance()->UsingCustomPark())
            {
                // in the park editor, we let the user overshoot, so he sticks to tight curves that are between two pieces
                ok = true;

                // we need to ensure that the rail segment is a good continuation of the segment that we were just on
                // Namely that one of the new start/end points is close to the end of the rail that we just came off
                // and that the direction we will be going along is within 45 degrees of the direction we were going along before. 
                // (NOT DONE HERE, as the following test is sufficent)

                // first a simple elimination, if the rail is longer than our velocity projected onto it then we can not posibly have overshot it!!
                Vertex	new_rail_segment = RailManager::GetPos(pNewEnd) - RailManager::GetPos(pNewStart);
                Vertex skater_movement = b - a;
                float new_rail_length = new_rail_segment.Length();
                if (new_rail_length > skater_movement.Length())
                {
                    ok = false;
                }

                // now we could find the shortest distance between two line segments; should be within 2 inches
                // (ALSO NOT DONE)

                // bit of a patch for the park editor, it's gone past the end of the rail, so set him in the middle of the rail, so we can continue nicely
                // possible glitch here, but better than falling of the rail
                // It's going to be a small rail anyway, so won't look too bad.
                if (ok && mid_dot >= 0.0f)
                {
                    rail_pos = (RailManager::GetPos(pNewEnd) + RailManager::GetPos(pNewStart)) / 2.0f;
                }

            }

            if (ok)
            {
                Vertex	newdir = RailManager::GetPos(pNewEnd) - RailManager::GetPos(pNewStart);
                newdir.Normalize();
                if (*GetVelocity()[X] == 0.0f && *GetVelocity()[Z] == 0.0f)
                {
                    *GetVelocity()[X] = GetMatrix().m[Z][X];
                    *GetVelocity()[Z] = GetMatrix().m[Z][Z];
                }

                // sign is which way we are going along the rail
                float sign = Sgn(D3DXVec3Dot(&newdir, GetVelocity()));
                ((Vertex*)GetVelocity())->RotateToNormal(newdir);
                *GetVelocity() *= sign;

                mp_rail_node = pNode;		// oh yes, this is the node
                *GetPosition() = rail_pos;			// move to closest position on the line
                maybe_trip_rail_trigger(Node::TRIGGER_SKATE_ONTO);
                set_terrain(mp_rail_node->GetTerrain());

                really_off = false;
            }
            else
            {
                // new position is outside this rail
            }
        }
        else
        {
            // its the same as this one
        }
    }
    else
    {
        // did not find any
    }

    if (!really_off) return;

    maybe_trip_rail_trigger(Node::TRIGGER_SKATE_OFF);
    SetState(Skater::AIR);
    *GetPosition()[Y] += 1.0f;
    FlagException("OffRail");
}


#endif